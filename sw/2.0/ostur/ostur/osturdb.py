import collections
import sqlite3
import time
from statistics import mean
from datetime import datetime

SAMPLE_PERIOD_S = 60
WEEK_TIME_DELTA_S = SAMPLE_PERIOD_S * 7
MONTH_TIME_DELTA_S = SAMPLE_PERIOD_S * 31

# timestamp and uid must be the first two
data_columns = [
    "timestamp",
    "uid",
    "temperature",
    "humidity",
    "pressure",
    "battery",
    "rssi",
]


class KeyValueStore(collections.MutableMapping):
    """ From https://stackoverflow.com/questions/47237807/use-sqlite-as-a-keyvalue-store """

    def __init__(self, filename=None, conn=None):
        self.filename = filename

        if filename is None:
            self.conn = conn
        else:
            self.conn = sqlite3.connect(filename)

        self.conn.execute("CREATE TABLE IF NOT EXISTS kv (key text unique, value text)")
        self.c = self.conn.cursor()

    def close():
        self.conn.commit()
        if self.filename is not None:
            self.conn.close()

    def __len__(self):
        self.c.execute("SELECT COUNT(*) FROM kv")
        rows = self.c.fetchone()[0]
        return rows if rows is not None else 0

    def iterkeys(self):
        c1 = self.conn.cursor()
        for row in c1.execute("SELECT key FROM kv"):
            yield row[0]

    def itervalues(self):
        c2 = self.conn.cursor()
        for row in c2.execute("SELECT value FROM kv"):
            yield row[0]

    def iteritems(self):
        c3 = self.conn.cursor()
        for row in c3.execute("SELECT key, value FROM kv"):
            yield row[0], row[1]

    def keys(self):
        return list(self.iterkeys())

    def values(self):
        return list(self.itervalues())

    def items(self):
        return list(self.iteritems())

    def __contains__(self, key):
        self.c.execute("SELECT 1 FROM kv WHERE key = ?", (key,))
        return self.c.fetchone() is not None

    def __getitem__(self, key):
        self.c.execute("SELECT value FROM kv WHERE key = ?", (key,))
        item = self.c.fetchone()
        if item is None:
            raise KeyError(key)
        return item[0]

    def __setitem__(self, key, value):
        self.c.execute("REPLACE INTO kv (key, value) VALUES (?,?)", (key, value))
        self.conn.commit()

    def __delitem__(self, key):
        if key not in self:
            raise KeyError(key)
        self.c.execute("DELETE FROM kv WHERE key = ?", (key,))
        self.conn.commit()

    def __iter__(self):
        return self.iteritems()


class OsturDB:
    def __init__(self, filename):
        self.conn = sqlite3.connect(filename)

        if self.conn is None:
            raise IOError("Unable to open sqlite database")

        self.cur = self.conn.cursor()

        self.OsturRecord = collections.namedtuple("OsturRecord", ["id"] + data_columns)

        self.tables = {
            "day": "day_samples",
            "week": "week_samples",
            "month": "month_samples",
        }

        self.__init_tables()

        self.devices = {}
        self.__load_devices()

        self.config = KeyValueStore(conn=self.conn)

        self.week_start = {}
        self.month_start = {}

        for device in self.devices:
            if "{}_week_start".format(device) in self.config:
                self.week_start[device] = int(
                    self.config["{}_week_start".format(device)]
                )

            if "{}_month_start".format(device) in self.config:
                self.month_start[device] = int(
                    self.config["{}_month_start".format(device)]
                )

    def close(self):
        self.__commit()
        self.conn.close()

    def __init_tables(self):
        # Table to store daily data (every minute)
        self.conn.execute(
            "CREATE TABLE IF NOT EXISTS "
            + "day_samples(id INTEGER PRIMARY KEY, timestamp INTEGER, uid INTEGER, "
            + "{} FLOAT)".format(" FLOAT, ".join(data_columns[2:]))
        )

        # Table to store weekly data (7 minute average)
        self.conn.execute(
            "CREATE TABLE IF NOT EXISTS "
            + "week_samples(id INTEGER PRIMARY KEY, timestamp INTEGER, uid INTEGER, "
            + "{} FLOAT)".format(" FLOAT, ".join(data_columns[2:]))
        )

        # Table to store monthly data (31 minute average)
        self.conn.execute(
            "CREATE TABLE IF NOT EXISTS "
            + "month_samples(id INTEGER PRIMARY KEY, timestamp INTEGER, uid INTEGER, "
            + "{} FLOAT)".format(" FLOAT, ".join(data_columns[2:]))
        )

        # Table to store devices and locations (if available)
        self.conn.execute(
            "CREATE TABLE IF NOT EXISTS "
            + "devices(uid INTEGER PRIMARY KEY, name TEXT)"
        )

        # Save the new tables
        self.__commit()

    def __load_devices(self):
        query = "SELECT * FROM devices"

        self.cur.execute(query)
        rows = self.cur.fetchall()

        for row in rows:
            self.devices[row[0]] = row[1]

    def __add_device(self, uid, name=None):
        self.rename_device(uid, name, True)

        # Update downsampling time
        self.config["{}_week_start".format(uid)] = 0
        self.config["{}_month_start".format(uid)] = 0
        self.week_start[uid] = 0
        self.month_start[uid] = 0

    def __ostur_row_factory(self, cursor, row):
        return self.OsturRecord(*row)

    def rename_device(self, uid, name=None, new=False):
        if not new and uid not in self.devices:
            raise KeyError("Unknown device!")

        if name is None:
            name = uid

        query = """
        REPLACE INTO devices (uid, name) VALUES (?, ?)
        """
        args = (uid, name)

        # Sanitize user input
        # See https://bobby-tables.com/python
        self.cur.execute(query, args)

        self.devices[uid] = name

    def get_records(
        self, table, start_date=None, end_date=None, order=None, limit=None, uid=None
    ):
        if table not in self.tables:
            raise KeyError("Invalid table!")

        self.cur.row_factory = self.__ostur_row_factory

        query = "SELECT * FROM {}".format(self.tables[table])
        args = []

        options = []
        if start_date is not None:
            options.append("timestamp >= ?")
            args.append(int(start_date))

        if end_date is not None:
            options.append("timestamp < ?")
            args.append(int(end_date))

        if uid is not None:
            options.append("uid == ?")
            args.append(uid)

        if len(options) > 0:
            query += " WHERE " + " AND ".join(options)

        if order == "desc":
            query += " ORDER BY timestamp DESC"

        if limit is not None:
            query += " LIMIT ?"
            args.append(int(limit))

        self.cur.execute(query, args)

        return self.cur.fetchall()

    def __insert_line(self, line, table="day"):
        query = "INSERT INTO {} VALUES(NULL,{})".format(
            self.tables[table], ",".join(["?"] * len(data_columns))
        )

        self.cur.execute(query, line)

    def __commit(self):
        retries = 5
        while retries > 0:
            try:
                self.conn.commit()
                break
            except sqlite3.OperationalError:
                retries -= 1
                continue

    def __downsample_check(self, timestamp, uid):
        """ See if current timestamp is outside of the latest
            averaging range. If so, downsample chunk and commit """

        if timestamp > (self.week_start[uid] + WEEK_TIME_DELTA_S):
            self.__downsample("week", uid)
            self.week_start[uid] = timestamp
            self.config["{}_week_start".format(uid)] = timestamp

        if timestamp > (self.month_start[uid] + MONTH_TIME_DELTA_S):
            self.__downsample("month", uid)
            self.month_start[uid] = timestamp
            self.config["{}_month_start".format(uid)] = timestamp

    def __downsample(self, table, uid):
        if table == "week":
            start_time = self.week_start[uid]
            end_time = start_time + WEEK_TIME_DELTA_S
        elif table == "month":
            start_time = self.month_start[uid]
            end_time = start_time + MONTH_TIME_DELTA_S
        else:
            raise ValueError("Invalid table!")

        query = """
            SELECT * FROM day_samples
            WHERE timestamp >= ?
            AND timestamp < ?
            AND uid == ?
            """
        args = (int(start_time), int(end_time), uid)

        self.cur.row_factory = self.__ostur_row_factory
        self.cur.execute(query, args)
        rows = self.cur.fetchall()

        if len(rows) == 0:
            # No rows to downsample!
            return

        lines = []
        for row in rows:
            line = []
            for key in data_columns:
                line.append(getattr(row, key))
            lines.append(line)

        avg_line = list(map(mean, zip(*lines)))

        # Round out the timestamp to seconds
        avg_line[data_columns.index("timestamp")] = int(
            avg_line[data_columns.index("timestamp")]
        )

        # Don't average the device number!
        avg_line[data_columns.index("uid")] = lines[0][data_columns.index("uid")]

        # Round the rest of the items
        for idx in range(2, len(avg_line)):
            avg_line[idx] = round(avg_line[idx], 3)

        # Save the data
        self.__insert_line(avg_line, table=table)

    def add_record(self, record, timestamp=None, commit=True):
        if getattr(record, "uid") not in self.devices:
            self.__add_device(getattr(record, "uid"))

        # If timestamp is none, use current time
        if timestamp is None:
            timestamp = int(time.time())

        line = [timestamp]

        for key in data_columns:
            if key != "timestamp":
                try:
                    line.append(getattr(record, key))
                except AttributeError:
                    line.append(0)

        self.__insert_line(line)

        self.__downsample_check(timestamp, getattr(record, "uid"))

        if commit:
            self.__commit()

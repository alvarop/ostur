from ostur.ostur_packet import OsturPacket

BLEOsturPacket = OsturPacket(
    "BLEOsturPacket",
    [
        ("addr", "6p"),
        ("uid", "I"),
        ("sample", "H"),
        ("temperature", "h"),
        ("humidity", "h"),
        ("battery", "H"),
        ("flags", "H"),
        ("magic", "H"),
        ("rssi", "b"),
        ("mfg_data_len", "b"),
        ("mfg_data", "H"),
    ],
)

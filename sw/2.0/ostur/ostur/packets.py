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
        ("reserved", "H"),
        ("rssi", "b"),
    ],
)

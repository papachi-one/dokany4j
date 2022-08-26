package one.papachi.dokany4j.constants;

public record FileAttributes(int fileAttributes) {

    public static final int FILE_ATTRIBUTE_READONLY = 0x00000001;
    public static final int FILE_ATTRIBUTE_HIDDEN = 0x00000002;
    public static final int FILE_ATTRIBUTE_SYSTEM = 0x00000004;
    public static final int FILE_ATTRIBUTE_DIRECTORY = 0x00000010;
    public static final int FILE_ATTRIBUTE_ARCHIVE = 0x00000020;
    public static final int FILE_ATTRIBUTE_DEVICE = 0x00000040;
    public static final int FILE_ATTRIBUTE_NORMAL = 0x00000080;
    public static final int FILE_ATTRIBUTE_TEMPORARY = 0x00000100;
    public static final int FILE_ATTRIBUTE_SPARSE_FILE = 0x00000200;
    public static final int FILE_ATTRIBUTE_REPARSE_POINT = 0x00000400;
    public static final int FILE_ATTRIBUTE_COMPRESSED = 0x00000800;
    public static final int FILE_ATTRIBUTE_OFFLINE = 0x00001000;
    public static final int FILE_ATTRIBUTE_NOT_CONTENT_INDEXED = 0x00002000;
    public static final int FILE_ATTRIBUTE_ENCRYPTED = 0x00004000;
    public static final int FILE_ATTRIBUTE_INTEGRITY_STREAM = 0x00008000;
    public static final int FILE_ATTRIBUTE_VIRTUAL = 0x00010000;
    public static final int FILE_ATTRIBUTE_NO_SCRUB_DATA = 0x00020000;
    public static final int FILE_ATTRIBUTE_EA = 0x00040000;
    public static final int FILE_ATTRIBUTE_PINNED = 0x00080000;
    public static final int FILE_ATTRIBUTE_UNPINNED = 0x00100000;
    public static final int FILE_ATTRIBUTE_RECALL_ON_OPEN = 0x00040000;
    public static final int FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS = 0x00400000;

    public boolean isFileAttributeReadOnly() {
        return (fileAttributes & FILE_ATTRIBUTE_READONLY) != 0;
    }

    public boolean isFileAttributeHidden() {
        return (fileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0;
    }

    public boolean isFileAttributeSystem() {
        return (fileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0;
    }

    public boolean isFileAttributeDirectory() {
        return (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
    }

    public boolean isFileAttributeArchive() {
        return (fileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0;
    }

    public boolean isFileAttributeDevice() {
        return (fileAttributes & FILE_ATTRIBUTE_DEVICE) != 0;
    }

    public boolean isFileAttributeNormal() {
        return (fileAttributes & FILE_ATTRIBUTE_NORMAL) != 0;
    }

    public boolean isFileAttributeTemporary() {
        return (fileAttributes & FILE_ATTRIBUTE_TEMPORARY) != 0;
    }

    public boolean isFileAttributeSparseFile() {
        return (fileAttributes & FILE_ATTRIBUTE_SPARSE_FILE) != 0;
    }

    public boolean isFileAttributeReparsePoint() {
        return (fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0;
    }

    public boolean isFileAttributeCompressed() {
        return (fileAttributes & FILE_ATTRIBUTE_COMPRESSED) != 0;
    }

    public boolean isFileAttributeOffline() {
        return (fileAttributes & FILE_ATTRIBUTE_OFFLINE) != 0;
    }

    public boolean isFileAttributeNotContentIndexed() {
        return (fileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) != 0;
    }

    public boolean isFileAttributeEncrypted() {
        return (fileAttributes & FILE_ATTRIBUTE_ENCRYPTED) != 0;
    }

    public boolean isFileAttributeIntegrityStream() {
        return (fileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM) != 0;
    }

    public boolean isFileAttributeVirtual() {
        return (fileAttributes & FILE_ATTRIBUTE_VIRTUAL) != 0;
    }

    public boolean isFileAttributeNoScrubData() {
        return (fileAttributes & FILE_ATTRIBUTE_NO_SCRUB_DATA) != 0;
    }

    public boolean isFileAttributeEa() {
        return (fileAttributes & FILE_ATTRIBUTE_EA) != 0;
    }

    public boolean isFileAttributePinned() {
        return (fileAttributes & FILE_ATTRIBUTE_PINNED) != 0;
    }

    public boolean isFileAttributeUnpinned() {
        return (fileAttributes & FILE_ATTRIBUTE_UNPINNED) != 0;
    }

    public boolean isFileAttributeRecallOnOpen() {
        return (fileAttributes & FILE_ATTRIBUTE_RECALL_ON_OPEN) != 0;
    }

    public boolean isFileAttributeRecallOnDataAccess() {
        return (fileAttributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS) != 0;
    }


}

package one.papachi.dokany4j.constants;

public record ShareAccess(int sharedAccess) {

    public static final int FILE_SHARE_READ = 1;
    public static final int FILE_SHARE_WRITE = 2;
    public static final int FILE_SHARE_DELETE = 4;

    public boolean isFileShareRead() {
        return (sharedAccess & FILE_SHARE_READ) > 0;
    }

    public boolean isFileShareWrite() {
        return (sharedAccess & FILE_SHARE_WRITE) > 0;
    }

    public boolean isFileShareDelete() {
        return (sharedAccess & FILE_SHARE_DELETE) > 0;
    }
}

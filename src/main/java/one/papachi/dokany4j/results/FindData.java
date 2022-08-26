package one.papachi.dokany4j.results;

public record FindData(String fileName, int dwFileAttributes, long ftCreationTime, long ftLastAccessTime,
                       long ftLastWriteTime, long fileSize) {
}

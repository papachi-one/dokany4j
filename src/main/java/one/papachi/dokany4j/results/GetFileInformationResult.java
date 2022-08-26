package one.papachi.dokany4j.results;

public record GetFileInformationResult(int status, int dwFileAttributes, int dwVolumeSerialNumber, long ftCreationTime,
                                       long ftLastAccessTime, long ftLastWriteTime, long fileIndex, long fileSize,
                                       int numberOfLinks) {
}

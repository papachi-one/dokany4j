package one.papachi.dokany4j.results;

public record DokanFileInfo(long context, boolean deleteOnClose, long dokanContext, boolean isDirectory,
                            boolean nocache, boolean pagingIo, int processId, boolean synchronousIo,
                            boolean writeToEndOfFile, int allocationUnitSize, long globalContext, String mountPoint,
                            int options, int sectorSize, boolean singleThread, int timeout, String uncName, short version) {
}

package one.papachi.dokany4j.results;

public record GetDiskFreeSpaceResult(int status, long freeBytesAvailable, long totalNumberOfBytes, long totalNumberOfFreeBytes) {
}

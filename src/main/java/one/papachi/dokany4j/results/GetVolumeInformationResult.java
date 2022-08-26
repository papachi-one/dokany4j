package one.papachi.dokany4j.results;

public record GetVolumeInformationResult(int status, String volumeName, int volumeSerialNumber, int maximumComponentLength, int fileSystemFlags, String fileSystemName) {
}

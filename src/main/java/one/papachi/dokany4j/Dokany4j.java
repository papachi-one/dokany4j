package one.papachi.dokany4j;

import one.papachi.dokany4j.results.CreateFileResult;
import one.papachi.dokany4j.results.DeleteDirectoryResult;
import one.papachi.dokany4j.results.DeleteFileResult;
import one.papachi.dokany4j.results.DokanFileInfo;
import one.papachi.dokany4j.results.FindFilesResult;
import one.papachi.dokany4j.results.FindStreamsResult;
import one.papachi.dokany4j.results.FlushFileBuffers;
import one.papachi.dokany4j.results.GetDiskFreeSpaceResult;
import one.papachi.dokany4j.results.GetFileInformationResult;
import one.papachi.dokany4j.results.GetFileSecurityResult;
import one.papachi.dokany4j.results.GetVolumeInformationResult;
import one.papachi.dokany4j.results.LockFileResult;
import one.papachi.dokany4j.results.MountedResult;
import one.papachi.dokany4j.results.MoveFileResult;
import one.papachi.dokany4j.results.ReadFileResult;
import one.papachi.dokany4j.results.SetAllocationSizeResult;
import one.papachi.dokany4j.results.SetEndOfFileResult;
import one.papachi.dokany4j.results.SetFileAttributesResult;
import one.papachi.dokany4j.results.SetFileSecurityResult;
import one.papachi.dokany4j.results.SetFileTimeResult;
import one.papachi.dokany4j.results.UnlockFileResult;
import one.papachi.dokany4j.results.UnmountedResult;
import one.papachi.dokany4j.results.WriteFileResult;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.file.Paths;

public class Dokany4j {

    private static final String temporaryLibraryPath = System.getProperty("java.io.tmpdir");

    private static final String library = "dokany4j.dll";

    static {
        File file = Paths.get(temporaryLibraryPath, library).toFile();
        try (InputStream is = Dokany4j.class.getResourceAsStream(library); OutputStream os = new FileOutputStream(file)) {
            is.transferTo(os);
        } catch (Exception e) {
        }
        System.load(file.getAbsolutePath());
    }

    public CreateFileResult createFile(String fileName, int genericDesiredAccess, int fileAttributesAndFlags,
                                       int shareAccess, int creationDisposition, DokanFileInfo dokanFileInfo) {
        return null;
    }

    public void closeFile(String fileName, DokanFileInfo dokanFileInfo) {

    }

    public void cleanup(String fileName, DokanFileInfo dokanFileInfo) {

    }

    public ReadFileResult readFile(String fileName, ByteBuffer buffer, long offset, DokanFileInfo dokanFileInfo) {
        return null;
    }

    public WriteFileResult writeFile(String fileName, ByteBuffer buffer, long offset, DokanFileInfo dokanFileInfo) {
        return null;
    }

    public FlushFileBuffers flushFileBuffers(String fileName, DokanFileInfo dokanFileInfo) {
        return null;
    }

    public GetFileInformationResult getFileInformation(String fileName, DokanFileInfo dokanFileInfo) {
        return null;
    }

    public FindFilesResult findFiles(String fileName, DokanFileInfo dokanFileInfo) {
        return null;
    }

    public DeleteFileResult deleteFile(String fileName, DokanFileInfo dokanFileInfo) {
        return null;
    }

    public DeleteDirectoryResult deleteDirectory(String fileName, DokanFileInfo dokanFileInfo) {
        return null;
    }

    public MoveFileResult moveFile(String fileName, String newFileName, boolean replaceIfExisting,
                                   DokanFileInfo dokanFileInfo) {
        return null;
    }

    public LockFileResult lockFile(String fileName, long offset, long length, DokanFileInfo dokanFileInfo) {
        return null;
    }

    public SetEndOfFileResult setEndOfFile(String fileName, long offset, DokanFileInfo dokanFileInfo) {
        return null;
    }

    public SetAllocationSizeResult setAllocationSize(String fileName, long allocSize, DokanFileInfo dokanFileInfo) {
        return null;
    }

    public SetFileAttributesResult setFileAttributes(String fileName, int fileAttributes, DokanFileInfo dokanFileInfo) {
        return null;
    }

    public SetFileTimeResult setFileTime(String fileName, long creationTime, long lastAccessTime, long lastWriteTime,
                                         DokanFileInfo dokanFileInfo) {
        return null;
    }

    public UnlockFileResult unlockFile(String fileName, long offset, long length, DokanFileInfo dokanFileInfo) {
        return null;
    }

    public GetFileSecurityResult getFileSecurity(String fileName, int securityInformation, ByteBuffer security,
                                                 DokanFileInfo dokanFileInfo) {
        return null;
    }

    public SetFileSecurityResult setFileSecurity(String fileName, int securityInformation, ByteBuffer security,
                                                 DokanFileInfo dokanFileInfo) {
        return null;
    }

    public GetVolumeInformationResult getVolumeInformation(DokanFileInfo dokanFileInfo) {
        return null;
    }

    public GetDiskFreeSpaceResult getDiskFreeSpace(DokanFileInfo dokanFileInfo) {
        return null;
    }

    public FindStreamsResult findStreams(String fileName, DokanFileInfo dokanFileInfo) {
        return null;
    }

    public MountedResult mounted(String mountPoint, DokanFileInfo dokanFileInfo) {
        return null;
    }

    public UnmountedResult unmounted(DokanFileInfo dokanFileInfo) {
        return null;
    }

    public native int mount(int version, boolean singleThread, int options, String mountPoint, String uncName, int timeout,
                             int allocationUnitSize, int sectorSize);

    public static native void unmount(String mountPoint);

}

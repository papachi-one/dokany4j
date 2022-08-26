package one.papachi.dokany4j.constants;

public enum NtStatus {
    STATUS_SUCCESS(0),
    STATUS_ACCESS_DENIED(1),
    STATUS_OBJECT_PATH_NOT_FOUND(-1),
    STATUS_NOT_IMPLEMENTED(-1),
    STATUS_NOT_A_DIRECTORY(0xC0000103),
    STATUS_DIRECTORY_NOT_EMPTY(0xC0000101),
    STATUS_INTERNAL_ERROR(0xC00000E5),
    STATUS_NO_SUCH_FILE(0xC000000F),
    STATUS_OBJECT_NAME_COLLISION(0xC0000035),
    STATUS_OBJECT_NAME_NOT_FOUND(0xC0000034),
    STATUS_END_OF_FILE(0xC0000011)
    ;

    private int status;

    private NtStatus(int status) {
        this.status = status;
    }

    public int getStatus() {
        return status;
    }
}

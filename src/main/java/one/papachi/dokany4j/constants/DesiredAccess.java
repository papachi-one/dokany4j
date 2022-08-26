package one.papachi.dokany4j.constants;

public record DesiredAccess(int desiredAccess) {

    public static final int GENERIC_READ = 0x80000000;
    public static final int GENERIC_WRITE = 0x40000000;
    public static final int GENERIC_EXECUTE = 0x20000000;
    public static final int GENERIC_ALL = 0x10000000;

    public boolean isGenericRead() {
        return (desiredAccess & GENERIC_READ) != 0;
    }

    public boolean isGenericWrite() {
        return (desiredAccess & GENERIC_WRITE) != 0;
    }

    public boolean isGenericExecute() {
        return (desiredAccess & GENERIC_EXECUTE) != 0;
    }

    public boolean isGenericAll() {
        return (desiredAccess & GENERIC_ALL) != 0;
    }

}

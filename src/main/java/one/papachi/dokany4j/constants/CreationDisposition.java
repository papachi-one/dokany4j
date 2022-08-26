package one.papachi.dokany4j.constants;

public record CreationDisposition(int createDisposition) {

    public static final int CREATE_NEW = 1;
    public static final int CREATE_ALWAYS = 2;
    public static final int OPEN_EXISTING = 3;
    public static final int OPEN_ALWAYS = 4;
    public static final int TRUNCATE_EXISTING = 5;

    public boolean isCreateNew() {
        return createDisposition == CREATE_NEW;
    }

    public boolean isCreateAlways() {
        return createDisposition == CREATE_ALWAYS;
    }

    public boolean isOpenExisting() {
        return createDisposition == OPEN_EXISTING;
    }

    public boolean isOpenAlways() {
        return createDisposition == OPEN_ALWAYS;
    }

    public boolean isTruncateExisting() {
        return createDisposition == TRUNCATE_EXISTING;
    }

}

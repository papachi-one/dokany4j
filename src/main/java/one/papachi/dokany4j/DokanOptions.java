package one.papachi.dokany4j;

public record DokanOptions(int dokanOptions) {

    public static final int DOKAN_OPTION_DEBUG = 1;
    public static final int DOKAN_OPTION_STDERR = 2;
    public static final int DOKAN_OPTION_ALT_STREAM = 4;
    public static final int DOKAN_OPTION_WRITE_PROTECT = 8;
    public static final int DOKAN_OPTION_NETWORK = 16;
    public static final int DOKAN_OPTION_REMOVABLE = 32;
    public static final int DOKAN_OPTION_MOUNT_MANAGER = 64;
    public static final int DOKAN_OPTION_CURRENT_SESSION = 128;
    public static final int DOKAN_OPTION_FILELOCK_USER_MODE = 256;
    public static final int DOKAN_OPTION_ENABLE_NOTIFICATION_API = 512;
    public static final int DOKAN_OPTION_DISABLE_OPLOCKS = 1024;
    public static final int DOKAN_OPTION_ENABLE_FCB_GARBAGE_COLLECTION = 2048;
    public static final int DOKAN_OPTION_CASE_SENSITIVE = 4096;
    public static final int DOKAN_OPTION_ENABLE_UNMOUNT_NETWORK_DRIVE = 8192;

    public static class Builder {

        private int dokanOptions;

        public DokanOptions build() {
            return new DokanOptions(dokanOptions);
        }

        public Builder setDokanOptionDebug() {
            dokanOptions |= DOKAN_OPTION_DEBUG;
            return this;
        }

        public Builder setDokanOptionStderr() {
            dokanOptions |= DOKAN_OPTION_STDERR;
            return this;
        }

        public Builder setDokanOptionAltStream() {
            dokanOptions |= DOKAN_OPTION_ALT_STREAM;
            return this;
        }

        public Builder setDokanOptionWriteProtect() {
            dokanOptions |= DOKAN_OPTION_WRITE_PROTECT;
            return this;
        }

        public Builder setDokanOptionNetwork() {
            dokanOptions |= DOKAN_OPTION_NETWORK;
            return this;
        }

        public Builder setDokanOptionRemovable() {
            dokanOptions |= DOKAN_OPTION_REMOVABLE;
            return this;
        }

        public Builder setDokanOptionMountManager() {
            dokanOptions |= DOKAN_OPTION_MOUNT_MANAGER;
            return this;
        }

        public Builder setDokanOptionCurrentSession() {
            dokanOptions |= DOKAN_OPTION_CURRENT_SESSION;
            return this;
        }

        public Builder setDokanOptionFilelockUserMode() {
            dokanOptions |= DOKAN_OPTION_FILELOCK_USER_MODE;
            return this;
        }

        public Builder setDokanOptionEnableNotificationApi() {
            dokanOptions |= DOKAN_OPTION_ENABLE_NOTIFICATION_API;
            return this;
        }

        public Builder setDokanOptionDisableOplocks() {
            dokanOptions |= DOKAN_OPTION_DISABLE_OPLOCKS;
            return this;
        }

        public Builder setDokanOptionEnableFcbGarbageCollection() {
            dokanOptions |= DOKAN_OPTION_ENABLE_FCB_GARBAGE_COLLECTION;
            return this;
        }

        public Builder setDokanOptionCaseSensitive() {
            dokanOptions |= DOKAN_OPTION_CASE_SENSITIVE;
            return this;
        }

        public Builder setDokanOptionEnableUnmountNetworkDrive() {
            dokanOptions |= DOKAN_OPTION_ENABLE_UNMOUNT_NETWORK_DRIVE;
            return this;
        }

    }

    public boolean isDokanOptionDebug() {
        return (dokanOptions & DOKAN_OPTION_DEBUG) != 0;
    }

    public boolean isDokanOptionStderr() {
        return (dokanOptions & DOKAN_OPTION_STDERR) != 0;
    }

    public boolean isDokanOptionAltStream() {
        return (dokanOptions & DOKAN_OPTION_ALT_STREAM) != 0;
    }

    public boolean isDokanOptionWriteProtect() {
        return (dokanOptions & DOKAN_OPTION_WRITE_PROTECT) != 0;
    }

    public boolean isDokanOptionNetwork() {
        return (dokanOptions & DOKAN_OPTION_NETWORK) != 0;
    }

    public boolean isDokanOptionRemovable() {
        return (dokanOptions & DOKAN_OPTION_REMOVABLE) != 0;
    }

    public boolean isDokanOptionMountManager() {
        return (dokanOptions & DOKAN_OPTION_MOUNT_MANAGER) != 0;
    }

    public boolean isDokanOptionCurrentSession() {
        return (dokanOptions & DOKAN_OPTION_CURRENT_SESSION) != 0;
    }

    public boolean isDokanOptionFilelockUserMode() {
        return (dokanOptions & DOKAN_OPTION_FILELOCK_USER_MODE) != 0;
    }

    public boolean isDokanOptionEnableNotificationApi() {
        return (dokanOptions & DOKAN_OPTION_ENABLE_NOTIFICATION_API) != 0;
    }

    public boolean isDokanOptionDisableOplocks() {
        return (dokanOptions & DOKAN_OPTION_DISABLE_OPLOCKS) != 0;
    }

    public boolean isDokanOptionEnableFcbGarbageCollection() {
        return (dokanOptions & DOKAN_OPTION_ENABLE_FCB_GARBAGE_COLLECTION) != 0;
    }

    public boolean isDokanOptionCaseSensitive() {
        return (dokanOptions & DOKAN_OPTION_CASE_SENSITIVE) != 0;
    }

    public boolean isDokanOptionEnableUnmountNetworkDrive() {
        return (dokanOptions & DOKAN_OPTION_ENABLE_UNMOUNT_NETWORK_DRIVE) != 0;
    }

}

package one.papachi.dokany4j.results;

import java.util.Iterator;

public record FindFilesResult(int status, Iterator<FindData> result) {
}

package bacteria.microscope.model;

public class ExceptionLog {

	private long key;
	private String fileName;
	private String filePath;

	private ExceptionLog(long key, String fileName, String filePath) {
		this.key = key;
		this.fileName = fileName;
		this.filePath = filePath;
	}

	public long getKey() {
		return key;
	}

	public String getFilePath() {
		return filePath;
	}

	public String getFileName() {
		return fileName;
	}

	public void setFileName(String fileName) {
		this.fileName = fileName;
	}

	public static ExceptionLog of(String fileName, String filePath) {
		return new ExceptionLog(System.nanoTime(), fileName, filePath);
	}
}

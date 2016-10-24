package bacteria.microscope.model;


public class ExceptionLog {

	private long key;
	private String filePath;
	
	private ExceptionLog(long key, String filePath) {
		this.key = key;
		this.filePath = filePath;
	}
	
	public long getKey() {
		return key;
	}

	public String getFilePath() {
		return filePath;
	}

	public static ExceptionLog of(String filePath) {
		return new ExceptionLog(System.nanoTime(), filePath);
	}
}

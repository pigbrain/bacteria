
public class Test {


	public static void main(String[] args) {

		System.out.println("HelloJava");
		
		a();
	}
	
	public static void a() {
		b();
	}
	
	public static void b() {
		int a = 10;
		int b = 11;
		int c = 13;
		
		c(-213, "kaka", 1, 2L);
	}
	
	public static void c(int param1, String param2, Integer param3, Long param4) {

		int a = 10;
		int b = 11;
		int c = 13;
		String d = "hello~!";
		
		try {
			System.out.println(a/ 0);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}

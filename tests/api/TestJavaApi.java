public class TestJavaApi {
    static {
        System.loadLibrary("Juneiform");
    }

    public static void main(String[] args) {
        System.out.println("JAVA api test");
        System.out.println("version: " + Juneiform.VERSION);
        System.out.println("build:   " + Juneiform.BUILD_NUMBER);

        FormatOptions fopts = new FormatOptions();
        fopts.setLanguage(Juneiform.LANGUAGE_RUSSIAN);

        RecognitionOptions ropts = new RecognitionOptions();
        ropts.setLanguage(Juneiform.LANGUAGE_RUSSIAN);

        Page p = Juneiform.recognize("@CMAKE_SOURCE_DIR@/images/test.bmp", ropts, fopts);
        p.save("/dev/stdout", Juneiform.FORMAT_TEXT);
    }
};

import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletIndustrialDigitalIn4V2;

public class TestWiegandGetter {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;
	private static final String UID = "HXg";

	private static void interpretWiegand26Data(boolean[] data) {
		if (data.length != 26) {
			System.out.println("Bad Length " + data.length);
			return;
		}

		int pe = 0;
		for (int i = 0; i < 13; ++i) {
			pe = pe ^ (data[i] ? 1 : 0);
		}

		int po = 0;
		for (int i = 13; i < 26; ++i) {
			po = po ^ (data[i] ? 1 : 0);
		}

		if (pe != 0 || po != 1) {
			System.out.println("Paritiy Error");
			return;
		}

		int fc = 0;
		for (int i = 1; i < 9; ++i) {
			fc = fc << 1;
			fc = fc | (data[i] ? 1 : 0);
		}

		int cc = 0;
		for (int i = 9; i < 25; ++i) {
			cc = cc << 1;
			cc = cc | (data[i] ? 1 : 0);
		}

		System.out.println("Facility Code " + fc + ", Card Number " + cc);
	}

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialDigitalIn4V2 idi4 =
		  new BrickletIndustrialDigitalIn4V2(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		idi4.addWiegandErrorCountListener(new BrickletIndustrialDigitalIn4V2.WiegandErrorCountListener() {
			public void wiegandErrorCount(long framingErrorCount, long overflowErrorCount) {
				System.out.println("Framing Error Count " + framingErrorCount + ", Overflow Error Count " + overflowErrorCount);
			}
		});

		idi4.setWiegandReaderConfig(true, 26, 50); // 26 bit, with 50 ms timeout
		idi4.setWiegandCallbackConfig(false, false, true);

		while (true) {
			Thread.sleep(100);

			boolean[] data = idi4.readWiegandData();

			if (data.length > 0) {
				interpretWiegand26Data(data);
			}
		}

		//ipcon.disconnect();
	}
}

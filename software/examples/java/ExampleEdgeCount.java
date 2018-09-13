import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletIndustrialDigitalIn4V2;

public class ExampleEdgeCount {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialDigitalIn4V2 idi4 =
		  new BrickletIndustrialDigitalIn4V2(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Configure rising edge count for channel 3 with 10ms debounce
		idi4.setEdgeCountConfiguration(3, 0, 10);

		// Get edge count 10 times with 1s delay
		for(int i = 0; i < 10; i++) {
			Thread.sleep(1000);

			// Get current edge count
			long count = idi4.getEdgeCount(3, false); // Can throw com.tinkerforge.TimeoutException
			System.out.println("County: " + count);
		}

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}

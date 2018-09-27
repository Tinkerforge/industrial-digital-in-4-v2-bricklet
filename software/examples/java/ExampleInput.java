import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletIndustrialDigitalIn4V2;

public class ExampleInput {
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

		// Get current value
		boolean[] value = idi4.getValue(); // Can throw com.tinkerforge.TimeoutException

		System.out.println("Channel 0: " + value[0]);
		System.out.println("Channel 1: " + value[1]);
		System.out.println("Channel 2: " + value[2]);
		System.out.println("Channel 3: " + value[3]);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}

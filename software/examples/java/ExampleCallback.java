import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletIndustrialDigitalIn4V2;

public class ExampleCallback {
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

		// Add value listener
		idi4.addValueListener(new BrickletIndustrialDigitalIn4V2.ValueListener() {
			public void value(int channel, boolean changed, boolean value) {
				System.out.println("Channel: " + channel);
				System.out.println("Changed: " + changed);
				System.out.println("Value: " + value);
				System.out.println("");
			}
		});

		// Set period for value (channel 1) callback to 0.1s (100ms)
		idi4.setValueCallbackConfiguration(1, 100, false);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}

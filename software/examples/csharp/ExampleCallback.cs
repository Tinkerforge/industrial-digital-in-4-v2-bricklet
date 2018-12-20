using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

	// Callback function for value callback
	static void ValueCB(BrickletIndustrialDigitalIn4V2 sender, byte channel, bool changed,
	                    bool value)
	{
		if(channel == BrickletIndustrialDigitalIn4V2.CHANNEL_0)
		{
			Console.WriteLine("Channel: 0");
		}
		else if(channel == BrickletIndustrialDigitalIn4V2.CHANNEL_1)
		{
			Console.WriteLine("Channel: 1");
		}
		else if(channel == BrickletIndustrialDigitalIn4V2.CHANNEL_2)
		{
			Console.WriteLine("Channel: 2");
		}
		else if(channel == BrickletIndustrialDigitalIn4V2.CHANNEL_3)
		{
			Console.WriteLine("Channel: 3");
		}

		Console.WriteLine("Changed: " + changed);
		Console.WriteLine("Value: " + value);
		Console.WriteLine("");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialDigitalIn4V2 idi4 =
		  new BrickletIndustrialDigitalIn4V2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register value callback to function ValueCB
		idi4.ValueCallback += ValueCB;

		// Set period for value (channel 1) callback to 0.1s (100ms)
		idi4.SetValueCallbackConfiguration(1, 100, false);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}

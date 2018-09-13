using System;
using System.Threading;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialDigitalIn4V2 idi4 =
		  new BrickletIndustrialDigitalIn4V2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Configure rising edge count for channel 3 with 10ms debounce
		idi4.SetEdgeCountConfiguration(3, 0, 10);

		// Get edge count 10 times with 1s delay
		for(int i = 0; i < 10; i++)
		{
			Thread.Sleep(1000);

			// Get current edge count
			long count = idi4.GetEdgeCount(3, false);
			Console.WriteLine("Count: " + count);
		}

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}

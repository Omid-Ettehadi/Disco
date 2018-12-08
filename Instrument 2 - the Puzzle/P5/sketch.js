/* -------------------- Variables -------------------- */
// Server variables
var dataServer;
var pubKey = 'pub-c-0a3b6b6e-0ef0-4b11-b8eb-1a597dc2331f';
var subKey = 'sub-c-c92fc548-f343-11e8-b085-b2b44c5b7fba';
var musicChannelName = "musicChannel";

// Serial Connection variables
var serial;       						// Serial port object
var serialPortName = "COM8";			// Serial port name
var ardSend = {};						// JSON variable

// Variables
var switch1val = 0, switch2val = 0, switch3val = 0, switch4val = 0, switch5val = 0, switch6val = 0;
var whoAmI = "thePuzzle";

/* -------------------- Functions -------------------- */
function setup() 
{
    getAudioContext().resume();
    createCanvas(windowWidth, windowHeight);
    background(0);
    
    // Initialize pubnub
    dataServer = new PubNub(
    {
        publish_key   : pubKey,
        subscribe_key : subKey,  
        ssl: true
    });
    
    // Attach callbacks to the pubnub object to handle messages and connections
    dataServer.addListener({message: readIncoming})
    dataServer.subscribe({channels: [musicChannelName]});

    // Setting up the serial port
    serial = new p5.SerialPort();		// Create the serial port object
    serial.open(serialPortName);		// Open the serialport
    serial.on('open',ardCon);			// Open the socket connection
    serial.on('data',dataReceived);		// Call dataReceived when new data is received
}
function draw() {}
function windowResized() {}

/* -------------------- PubNub -------------------- */
// Send data to pubnub
function sendTheMessage() 
{
	dataServer.publish(
    {
        channel: musicChannelName,
        message: 
        {
			who: whoAmI,
			switch1: switch1val,
			switch2: switch2val,
			switch3: switch3val,
			switch4: switch4val,
			switch5: switch5val,
			switch6: switch6val,
        }
    });
}

// Read data from pubnub
function readIncoming(inMessage) {}

/* -------------------- Arduino -------------------- */
// Recieve data from Arduino
function dataReceived ()
{
	var rawData = serial.readStringUntil('\r\n');		// Read the incoming string until it sees a newline
	//console.log(rawData);
	
	if( rawData.length > 1 )								// Check that there is something in the string
	{
		switch1val = JSON.parse(rawData).switch1;		// Set variable switch1val to switch1 from the incoming string 
		switch2val = JSON.parse(rawData).switch2;		// Set variable switch2val to switch2 from the incoming string 
		switch3val = JSON.parse(rawData).switch3;		// Set variable switch3val to switch3 from the incoming string 
		switch4val = JSON.parse(rawData).switch4;		// Set variable switch4val to switch4 from the incoming string 
		switch5val = JSON.parse(rawData).switch5;		// Set variable switch5val to switch5 from the incoming string 
		switch6val = JSON.parse(rawData).switch6;		// Set variable switch6val to switch6 from the incoming string
		sendTheMessage();
	}
}
function ardCon ()
{
    console.log("connected to the Arduino Micro! Let's Go");
}
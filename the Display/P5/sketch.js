/* -------------------- Variables -------------------- */
// Server variables
var dataServer;
var pubKey = 'pub-c-0a3b6b6e-0ef0-4b11-b8eb-1a597dc2331f';
var subKey = 'sub-c-c92fc548-f343-11e8-b085-b2b44c5b7fba';
var musicChannelName = "musicChannel";

// Serial Connection variables
var serial;       						// Serial port object
var serialPortName = "COM7";			// Serial port name
var ardSend = {};						// JSON variable


/* -------------------- Functions -------------------- */
function preload(){
	// Preload sounds
	//soundArticle1 = loadSound("audios/.mp3");
}
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
    dataServer.addListener({ message: readIncoming, presence: whoisconnected })
    dataServer.subscribe({channels: [musicChannelName]});

    // Setting up the serial port
    serial = new p5.SerialPort();		// Create the serial port object
    serial.open(serialPortName);		// Open the serialport
    serial.on('open',ardCon);			// Open the socket connection
}

function draw() 
{
    
}

function windowResized() {}

/* -------------------- PubNub -------------------- */
// Send data to pubnub
function sendTheMessage() {}

// Read data from pubnub
function readIncoming(inMessage) 
{
    if(inMessage.channel == musicChannelName)
    {
		if ( inMessage.message.whoAmI == "Game" )
		{

		} else if ( inMessage.message.whoAmI == "Piano" ) 
		{

		} else if ( inMessage.message.whoAmI == "Ball" ) 
		{

		}
    }
}

function whoisconnected(connectionInfo) {}

/* -------------------- Arduino -------------------- */
// Send data to Arduino
function sendData()
{
    ardSend.LEDStatus = 							// Add the value to the LEDStatus parameter on the json object
    
	var sendString = JSON.stringify(ardSend);   	// Convert the json to a string  
    console.log(sendString)

    serial.write(sendString);                  		// Send it over the serial port    
    serial.write('\n');                           	// Write a new line character
}

function ardCon()
{
    console.log("connected to the Arduino Micro! Let's Go");
}
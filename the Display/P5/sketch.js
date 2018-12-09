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

// PubNub Data
var BallOne_averageVelocity = 0, Prev_BallOne_averageVelocity = 0;
var BallTwo_averageVelocity= 0, Prev_BallTwo_averageVelocity = 0;
var switch1val = 0, switch2val = 0, switch3val = 0,
	switch4val = 0, switch5val = 0, switch6val = 0;

// Arduino Data
var PuzzleStatus = 0, BallOneStatus = 0, BallTwoStatus = 0;

// Audios
var BigCircleAudio, SmallCircleAudio;
var BigRectAudio, SmallRectAudio;
var TrigAudio, TrapAudio;
var BallOneAudio, BallTwoAudio;
var volume1 , volume2, volume3, volume4, volume5, volume6;

/* -------------------- Functions -------------------- */
function preload(){
	// Preload sounds
	BigCircleAudio = loadSound("audios/KickTrack.wav");
	TrapAudio = loadSound("audios/FluteTrack1.wav");
	SmallRectAudio = loadSound("audios/BassTrack.wav");
	SmallCircleAudio = loadSound("audios/FluteTrack2.wav");
	TrigAudio = loadSound("audios/PikesTrack.wav");
	BigRectAudio = loadSound("audios/VocalTrack.wav");
	
	BallOneAudio = loadSound("audios/GongTrack.wav");
	BallTwoAudio = loadSound("audios/TambourineTrack.wav");
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
    dataServer.addListener({message: readIncoming})
    dataServer.subscribe({channels: [musicChannelName]});

	// Setting up the serial port
    serial = new p5.SerialPort();		// Create the serial port object
    serial.open(serialPortName);		// Open the serialport
    serial.on('open',ardCon);			// Open the socket connection
}
function draw() 
{
	// -------------------- thePuzzle -------------------- //
	// Play all tracks on a constant loop
	if ( !BigCircleAudio.isPlaying() ) 
	{
		memory = 0;
		BigCircleAudio.play();
    }
	if ( !TrapAudio.isPlaying() ) 
	{
		TrapAudio.play();
    }
	if ( !SmallRectAudio.isPlaying() ) 
	{
		SmallRectAudio.play();  
    }
	if ( !SmallCircleAudio.isPlaying() ) 
	{
		SmallCircleAudio.play();
	}
	if ( !TrigAudio.isPlaying() ) 
	{
		TrigAudio.play();  
    }
	if ( !BigRectAudio.isPlaying() ) 
	{
		BigRectAudio.play();
    }
	
	// Set the track's volume according to switches
	if ( switch1val == 1 ) { volume1 = 1; } else { volume1 = 0; }
    BigCircleAudio.setVolume(volume1);  
	if ( switch2val == 1 ) { volume2 = 1; } else { volume2 = 0; }
    TrapAudio.setVolume(volume2);
	if ( switch3val == 1 ) { volume3 = 1; } else { volume3 = 0; }
    SmallRectAudio.setVolume(volume3);
	if ( switch4val == 1 ) { volume4 = 1; } else { volume4 = 0; }
	SmallCircleAudio.setVolume(volume4);  
	if ( switch5val == 1 ) { volume5 = 1; } else { volume5 = 0; }  
    TrigAudio.setVolume(volume5);
	if ( switch6val == 1 ) { volume6 = 1; } else { volume6 = 0; }
    BigRectAudio.setVolume(volume6);  
	
	// Set the status for arduino
	PuzzleStatus = (switch6val || switch5val || switch4val || switch3val|| switch2val|| switch1val);
	
	// -------------------- theBalls -------------------- //
	// the Ball 1
	// If the velocity is higher than 20 and the value is not the same as last trial play the sound
	// Set BallOneStatus to 1 if the music is playing for the arduino to show the light
	if ( BallOne_averageVelocity >= 20 &&  BallOne_averageVelocity != Prev_BallOne_averageVelocity){
		Prev_BallOne_averageVelocity = BallOne_averageVelocity;	
		if ( !BallOneAudio.isPlaying() )
		{
			BallOneAudio.play();
			BallOneAudio.setVolume(1); 
			BallOneStatus = 1;
		} 
	} else {
		BallOneStatus = 0;
	}
	// the Ball 2
	// If the velocity is higher than 20 and the value is not the same as last trial play the sound
	// Set BallTwoStatus to 1 if the music is playing for the arduino to show the light
	if ( BallTwo_averageVelocity >= 20 &&  BallTwo_averageVelocity != Prev_BallTwo_averageVelocity)
	{
		Prev_BallTwo_averageVelocity = BallTwo_averageVelocity;
		if ( !BallTwoAudio.isPlaying() )
		{
			BallTwoAudio.play();
			BallTwoAudio.setVolume(1); 
			BallTwoStatus = 1;
		} 
	} else
	{
		BallTwoStatus = 0;
	}
	
	// Send data to arduino
	sendData();
}
function windowResized() 
{
    resizeCanvas(windowWidth, windowHeight);
	background(0); 
}

/* -------------------- PubNub -------------------- */
// Send data to pubnub
function sendTheMessage() {}

// Read data from pubnub
function readIncoming(inMessage) 
{
    if(inMessage.channel == musicChannelName)
    {
		if ( inMessage.message.who == "thePuzzle" )
		{
			switch1val = inMessage.message.switch1;
			switch2val = inMessage.message.switch2;
			switch3val = inMessage.message.switch3;
			switch4val = inMessage.message.switch4;
			switch5val = inMessage.message.switch5;
			switch6val = inMessage.message.switch6;
		} 
		else if ( inMessage.message.who == "theBallOne" ) 
		{
			BallOne_averageVelocity = inMessage.message.averageVelocity;
		} 
		else if ( inMessage.message.who == "theBallTwo" ) 
		{
			BallTwo_averageVelocity = inMessage.message.averageVelocity;
		}
    }
}

/* -------------------- Arduino -------------------- */
// Send data to Arduino
function sendData()
{	
    ardSend.PuzzleStatus = PuzzleStatus;						// Add the value to the LEDOneStatus parameter on the json object
    ardSend.BallOneStatus = BallOneStatus;						// Add the value to the LEDTwoStatus parameter on the json object
	ardSend.BallTwoStatus = BallTwoStatus;						// Add the value to the LEDThreeStatus parameter on the json object

	var sendString = JSON.stringify(ardSend);   				// Convert the json to a string  
    //console.log(sendString)
	
	serial.write(sendString);                  					// Send it over the serial port    
    serial.write('\n');                           				// Write a new line character	
}

function ardCon()
{
    console.log("connected to the Arduino Micro! Let's Go");
}
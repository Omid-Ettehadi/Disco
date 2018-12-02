/* -------------------- Variables -------------------- */


/* -------------------- Functions -------------------- */
function setup() 
{
    getAudioContext().resume();
    createCanvas(windowWidth, windowHeight);
    background(0);
}
function draw() {}
function windowResized() {}

/* -------------------- PubNub -------------------- */
// Send data to pubnub
function sendTheMessage() {}

// Read data from pubnub
function readIncoming(inMessage) {}

/* -------------------- Arduino -------------------- */
// Recieve data from Arduino
function dataReceived () {}

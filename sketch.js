let dataServer;
let pubKey = 'pub-c-b1c6095a-ac3d-469e-b1ac-5a24129b7dc1';
let subKey = 'sub-c-1d4781da-915c-11eb-83ad-9af11ba6f334';
let myID = "receiver";
let channelName = "shoulder_feeling";

function preload(){
}

function setup() {
  dataServer = new PubNub({
    subscribe_key : subKey,  
    ssl: true,  
    uuid: myID
  });
  dataServer.addListener({ message: readIncoming });
  dataServer.subscribe({channels: [channelName]});
}


function draw() {
  
}
  
function readIncoming(inMessage){ 
  const msg = inMessage.message;
  const feeling = msg.feeling;
  if (feeling){
    console.log('Feeling: ', feeling);
    if (feeling == 1){
      document.getElementById('left').setAttribute('src', 'img/feeling1.png');
    }
    if (feeling == 2){
      document.getElementById('left').setAttribute('src', 'img/feeling2.png');
    }
    if (feeling == 3){
      document.getElementById('left').setAttribute('src', 'img/feeling3.png');
    }
    if (feeling == 4){
      document.getElementById('left').setAttribute('src', 'img/feeling4.png');
    }
    if (feeling == 5){
      document.getElementById('left').setAttribute('src', 'img/feeling5.png');
    }
  }
}
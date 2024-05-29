/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-websocket
 */

const char *HTML_CONTENT = R"=====(
<!DOCTYPE html>
<html>

<head>

  <title>Youtube Controller</title>

</head>

<style>
  body {
    font-size: 16px;
  }
  .websocket {
    display: flex;
    align-items: center;
    margin-bottom: 5px;
    font-size: 3vw;
  }
  .websocket button {
    background-color: #007bff;
    color: #fff;
    border: none;
    padding: 5px 10px;
    cursor: pointer;
    height: 10vh;
    width: 10vw;
    font-size: 2vw;
  }
  .websocket .label {
    margin-left: auto;
  }
  .titolo {
    font-size: 5vw;
    color: red;
  }
  .player{
    margin: auto;
    display: block;
  }
  .logo{
    height: 10%;
    width: 10%;
    margin-right: 10px;
  }
  .top{
    display: flex;
    justify-content: center;
    align-items: center;
    width: 100%;
  }
  .form{
    display: flex;
    justify-content: center;
    align-items: center;
    width: 100%;
    padding-bottom: 5%;
  }
  .searchBar{
    width: 60%;
  }

</style>

<body>
    
  <div class="top">
    <img src="https://upload.wikimedia.org/wikipedia/commons/e/ef/Youtube_logo.png" alt="logo" class="logo">
    <h2 class="titolo">Youtube Controller</h2>
  </div>
  <form id="myForm" class="form">
    <input class="searchBar" type="text" id="userInput" name="userInput" placeholder="Insert link" required>
    <button type="submit">Search</button>
  </form>
  <div id="player" class="player"></div>


  <script>

    var ws;
    var wsm_max_len = 4096;
    var lista = [];
    var url;

    var video;

    // This function creates an <iframe> (and YouTube player) after the API code downloads and opens a websocket connection
    function onYouTubeIframeAPIReady() {
      if(video == "playlist"){
        player = new YT.Player('player', {
          height: '720',
          width: '1280',
          playerVars: {
                      listType: 'playlist',
                      list: url
                  },
          events: {
            'onReady': onPlayerReady,
          }
        });
      }else if (video == "video"){
        player = new YT.Player('player', {
          height: '720',
          width: '1280',
          videoId: url,
          events: {
            'onReady': onPlayerReady,
          }
        });
      }

      ws = new WebSocket("ws://" + window.location.host + ":81");
      ws.onmessage = ws_onmessage;
    }

    function onPlayerStateChange(event) {
      if (event.data === YT.PlayerState.ENDED) {
        sendVideoInfo(); // Call function to send video info when video ends
      }
    }

    // Function to send video duration and title via WebSocket
    function sendVideoInfo() {
      var duration = player.getDuration(); 
      var title = player.getVideoData().title; 
      var message = {
        duration: duration,
        title: title
      };
      console.log(message);
      ws.send(JSON.stringify(message)); // Send message via WebSocket
    }

    // The API will call this function when the video player is ready.
    function onPlayerReady(event) {
      event.target.playVideo();
    }

    // When a message arrives through the websocket, it is pushed into the list
    function ws_onmessage(e_msg) {
      lista.push(e_msg);
    }

    //Functions to control the player variable
    function lowerVolume(actual_volume){
      actual_volume -= 5;
      if (actual_volume < 0){
        actual_volume = 0;
        }
      player.setVolume(actual_volume);
    }

    function higherVolume(actual_volume){
      actual_volume += 5;
      if (actual_volume > 100){
        actual_volume = 100;
        }
      player.setVolume(actual_volume);
    }

    function higherSpeed(actual_rate){
      const AVAILABLE_RATES = player.getAvailablePlaybackRates();
      var i = AVAILABLE_RATES.indexOf(actual_rate);
      if(actual_rate != AVAILABLE_RATES[AVAILABLE_RATES.length - 1]){
        player.setPlaybackRate(AVAILABLE_RATES[i+1]);
      }
    }

    function lowerSpeed(actual_rate){
      const AVAILABLE_RATES = player.getAvailablePlaybackRates();
      var i = AVAILABLE_RATES.indexOf(actual_rate);
      if(actual_rate != AVAILABLE_RATES[0]){
        player.setPlaybackRate(AVAILABLE_RATES[i-1]);
      }
    }

    function prevTime(actual_time){
      actual_time -= 10;
      if (actual_time > 0){ 
        player.seekTo(actual_time);
      }
      else{
        player.seekTo(0);
      }
    }

    function plusTime(actual_time){
      actual_time += 10;
      if(actual_time < player.getDuration()){
        player.seekTo(actual_time);
      }else{
        player.seekTo(player.getDuration());
      }
    }

    function switch_case(msg){
      switch(msg.data){
        case "volDown":
          lowerVolume(player.getVolume());
          break;

        case "volUp":
          higherVolume(player.getVolume());
          break;
                  
        case "timePlus":
          plusTime(player.getCurrentTime());
          break;
              
        case "timeMinus":
          prevTime(player.getCurrentTime());
          break;

        case "pause":
          player.pauseVideo();
          break;
                  
        case "play":
          player.playVideo();
          break;
                  
        case "mute":
          player.mute();
          break;
                  
        case "unmute":
          player.unMute();
          break;
                  
        case "prev":
          player.previousVideo();
          break;

        case "next":
          player.nextVideo();
          break;

        case "lSpee":
          lowerSpeed(player.getPlaybackRate());
          break;
                  
        case "hSpee":
          higherSpeed(player.getPlaybackRate());
          break;
      }
    }

    // Every 0.250 sec an item in the list is popped and inserted into the switch case function,
    // that will trigger the correct event
    const interval = setInterval(function() {
      if (lista.length > 0){
        switch_case(lista.pop())
      }
    }, 250);

    // Function to save user input into variable and create iframe player with url
    document.getElementById('myForm').addEventListener('submit', 
      function(event) {
        event.preventDefault();
        var userInput = document.getElementById('userInput').value;
        url = userInput.split('=')[2];
        if (url == undefined){
          video = "video";
          url = userInput.split('=')[1];
        }else{
          video = "playlist";
        }
        // This code loads the IFrame Player API code asynchronously.
        var tag = document.createElement('script');
        tag.src = "https://www.youtube.com/iframe_api";
        var firstScriptTag = document.getElementsByTagName('script')[0];
        firstScriptTag.parentNode.insertBefore(tag, firstScriptTag);
        
        var player;
      });

  </script>
</body>

</html>
)=====";
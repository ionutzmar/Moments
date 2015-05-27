import ddf.minim.*;
import processing.serial.*;

Serial myPort;
PImage background;
PImage playButton;
PImage pauseButton;
PImage forwardButton;
PImage backwardButton;
PFont font;
String oldPurpose;
String purpose = "";
boolean ask;
boolean write;
int cursorX;
long currentTime;
long lastTime = 0;
long playerPrevPos;
long lastTimeSong;
int temp;
int action; //which action to perform when receving data from arduino
boolean play = false;
boolean prevPlay = false;
int songNumber = 0;
String[] songs;
AudioPlayer player;
Minim minim;


void setup(){
  size(displayWidth, displayHeight, P2D);
  smooth();
  myPort = new Serial(this, Serial.list()[0], 9600);
  background = loadImage("background.jpg");
  playButton = loadImage("play.png");
  forwardButton = loadImage("forward.png");
  backwardButton = loadImage("backward.png");
  pauseButton = loadImage("pause.png");
  image(background, 0, 0, displayWidth, displayHeight);
  font = createFont("Arial", 100, true);
 
  String database[] = loadStrings("database.txt");
  oldPurpose = join(database, " ");
  if ( (oldPurpose.equals("")) || ((!oldPurpose.substring(0, oldPurpose.indexOf("@")).equals(str(day()))) || !oldPurpose.substring(oldPurpose.indexOf("@") + 1, oldPurpose.indexOf("#")).equals(str(month())) || !oldPurpose.substring(oldPurpose.indexOf("#") + 1, oldPurpose.indexOf("$")).equals(str(year()))))
    {
      ask = true;
      println(oldPurpose);
    }
   else
     purpose = oldPurpose.substring(oldPurpose.indexOf("$ ") + 1, oldPurpose.indexOf("^^"));


  File dir = new File("C:\\Users\\Mihalache\\Documents\\Mein IE Projekt\\sketch_150508a\\Music");
  songs = dir.list();
  
  if (songs == null) 
    println("Folder does not exist or cannot be accessed.");
  
  else 
    println(songs);

      
  minim = new Minim(this);
  player = minim.loadFile("Music\\" + songs[0]);

}
  
void draw(){
  background(255);
  image(background, 0, 0, displayWidth, displayHeight);
  currentTime = millis();
  textFont(font, 125);
  fill(251, 130, 1);
  //fill(255);
  textAlign(CENTER);
  
    
  if (myPort.available() > 1) 
   {
       temp = myPort.read();
       action = myPort.read();
     //kif (action > 0)
      //println(temp);
      //println(myPort.available());
     if (action == 1)
       play = !play;
     else if (action == 2)
       changeSong();
     else if (action == 3)
     {    
        backward();
        changeSong(); 
    }
    action = 0;
   }


   
  if (minute() / 10 != 0)
    text(hour() + ":" + minute(), width / 2, height - (height * 10 / 12));
  else
    text(hour() + ":0" + minute(), width / 2, height - (height * 10 / 12));
    
   if (ask)
   {
    textFont(font, 45);
    //fill(192, 255, 0);
    fill(249, 78, 56);
    textAlign(CENTER);
    text("What is your main purpose for today?", width / 2, height - (height * 9 / 12));
    //stroke(192, 255, 0);
    stroke(249, 78, 56);
    line(width * 3 / 12, height - (height * 8 / 12), width * 9 / 12,  height - (height * 8 / 12));
    text(purpose, width / 2, height - (height * 8.15 / 12));
    if (write)
    {
        currentTime = millis();
       if(currentTime - lastTime > 1000)
         lastTime = currentTime;
       if (currentTime - lastTime > 500)
         line(textWidth(purpose) / 2 + width / 2, height - (height * 8.1 / 12), textWidth(purpose) / 2 + width / 2, height - (height * 8.65 / 12));
    }
   }
   else 
   {
       textFont(font, 45);
       //fill(192, 255, 0);
       fill(249, 78, 56);
       textAlign(CENTER);
       text("Today,", width / 2, height - (height * 9 / 12 ));
       text(purpose, width / 2, height - (height * 8.15 / 12));
       stroke(249, 78, 56);
       line(width / 2 + textWidth(purpose) / 2 + 20, height - (height * 8.15 / 12), width / 2 + textWidth(purpose) / 2 + 27, height - (height * 8.15 / 12) - 7);
       line((width / 2 + textWidth(purpose) / 2 + 20), height - (height * 8.15 / 12) - 7, width / 2 + textWidth(purpose) / 2 + 27, height - (height * 8.15 / 12));
   }
   
   if (play)
     image(pauseButton, width / 2 - (width / 18 - 5) / 2, height * 9 / 12 - (width / 18 - 5) / 2, width / 18 - 5, width / 18);
   else
     image(playButton, width / 2 - (width / 18 - 5) / 2, height * 9 / 12 - (width / 18 - 5) / 2, width / 18 - 5, width / 18);
   image(forwardButton, width * 6 / 10 - (width / 18 - 5) / 2, height * 9 / 12 - (width / 18 - 5) / 2, width / 18 - 5, width / 18);
   image(backwardButton, width * 4 / 10 - (width / 18 - 5) / 2, height * 9 / 12 - (width / 18 - 5) / 2, width / 18 - 5, width / 18);
   
   
   if (play)
   {
     fill(249, 78, 56);
     textFont(font, 20);
     text("Now playing: " + songs[songNumber], width / 2, height * 10 / 12);
   }
   
   textFont(font, 45);
   //fill(249, 78, 56);
   fill(251, 130, 1);
   text("In your room right now: " + str(temp) + "°C", width / 2, height * 7 / 12);
   
    if (prevPlay != play)
    {
      
      if (play)
      {
         player.play();
         lastTimeSong = currentTime;
      }
      else
        player.pause();

        prevPlay = play;
    }
    if (play && playerPrevPos != player.position())
      lastTimeSong = currentTime;
      
    if (play && ((player.length() <= player.position() + 1 ) || currentTime - lastTimeSong > 1500))
      changeSong();
    
    playerPrevPos = player.position();
    println(player.length() - player.position());
    println(play);
    
      
 }
 
 void mousePressed()
{
  if (ask && mouseX > width * 3 / 12 && mouseX < width * 9 / 12 && mouseY < (height - (height * 7.5 / 12)) && mouseY > (height - (height * 9 / 12)))
    write = true;
  else
    write = false;
  if (ask == false && mouseX <= width / 2 + textWidth(purpose) / 2 + 27 && mouseX >= (width / 2 + textWidth(purpose) / 2 + 20) && mouseY >=  height - (height * 8.15 / 12) - 7 && mouseY <= height - (height * 8.15 / 12))
  {
    ask = true;
    purpose = "";
    write = true;
  }
  if (!play && mouseX <= width / 2 + (width / 20 - 5) && mouseX >= width / 2 - (width / 20 - 5) && mouseY >= height * 9 / 12 - (width / 18 - 5) && mouseY <= height * 9 / 12 + (width / 18 - 5))
    play = true;
  else if(play && mouseX <= width / 2 + (width / 20 - 5) && mouseX >= width / 2 - (width / 20 - 5) && mouseY >= height * 9 / 12 - (width / 18 - 5) && mouseY <= height * 9 / 12 + (width / 18 - 5))
    play = false;
  if (dist(mouseX, mouseY, width * 6 / 10, height * 9 / 12) < (width / 20))
      changeSong();
  if (dist(mouseX, mouseY, width * 4 / 10, height * 9 / 12) < (width / 20))
  {
    backward();
    changeSong(); 
  }
}

void keyPressed()
{
   if (write)
  {
    if (key == BACKSPACE && purpose.length() > 0)
      purpose = purpose.substring(0, purpose.length()-1);
    else if (key == ENTER || key == RETURN)
    {
      purpose = purpose + "!";
      saveStrings("data\\database.txt", split(day() + "@" + month() + "#" + year() + "$ " + purpose + "^^", " "));
      ask = false;
      write = false;
    }
    else if (key != CODED)
     purpose = purpose + key;
  } 
}

void changeSong()
    {
      if (play)
      {
       if (songNumber < songs.length - 1)
            songNumber++;
       else
         songNumber = 0;
        player.close();
        player = minim.loadFile("Music\\" + songs[songNumber]);
        player.play();
        lastTimeSong = currentTime;
      }
    }
    
void backward()
{
   if(songNumber > 1 && player.position() > 5000)
   {
          songNumber -= 1;
          println(1);
        }
       else if (songNumber > 1)
        {
          songNumber -= 2;
          println(2);
        }
     else if (player.position() > 5000)
        {
          songNumber = songs.length - 1 + songNumber;
          println(3);
        }
       else
        {
          songNumber = songs.length - 2 + songNumber;
          println(4);
    }
}

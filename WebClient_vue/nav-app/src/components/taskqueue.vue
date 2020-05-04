//task.vue
<template>
    <div>
        <h2>{{title}}</h2>
        <hr>
        <canvas id="Canvas" width="700" height="700"></canvas>
        <button @click="remMarks()">Remove marks</button>
    </div>
</template>

<script>
    export default{
        data(){
            return {
                title:"Task",
                sContent:"This is task components",
                remove:0
            }
        },
        methods: {
            remMarks: function () {
                this.remove =1;
            }
        },
        mounted() {

                //var canvas;
                //var context;
                //var mapSprite;
                let Markers;
                let canvas = document.getElementById('Canvas');
                let context = canvas.getContext("2d");
                // Map sprite
                let mapSprite = new Image();
                mapSprite.src = "https://i.ibb.co/VxrNxRG/test.jpg";
                //mapSprite.width = 1000;
                //mapSprite.height = 1000;

                let Marker = function () {
                    this.Sprite = new Image();
                    this.Sprite.src = "http://www.clker.com/cliparts/w/O/e/P/x/i/map-marker-hi.png"
                    this.Width = 12;
                    this.Height = 20;
                    this.XPos = 0;
                    this.YPos = 0;
                }

                Markers = new Array();

                let mouseClicked = function (mouse) {
                    // Get corrent mouse coords
                    var rect = canvas.getBoundingClientRect();
                    var mouseXPos = (mouse.x - rect.left);
                    var mouseYPos = (mouse.y - rect.top);

                    //console.log("Marker added");

                    // Move the marker when placed to a better location
                    var marker = new Marker();
                    marker.XPos = mouseXPos - (marker.Width / 2);
                    marker.YPos = mouseYPos - marker.Height;

                    Markers.push(marker);
                }

                // Add mouse click event listener to canvas
                canvas.addEventListener("mousedown", mouseClicked, false);
                context.font = "15px Georgia";
                context.textAlign = "center";


                let draw = function () {
                    // Clear Canvas
                    context.fillStyle = "#000";
                    context.fillRect(0, 0, canvas.width, canvas.height);

                    // Draw map
                    // Sprite, X location, Y location, Image width, Image height
                    // You can leave the image height and width off, if you do it will draw the image at default size
                    context.drawImage(mapSprite, 0, 0, 700, 700);

                    if (this.remove == 1) {
                        Markers.clear();
                        return;
                    }

                    // Draw markers
                    var markerText;
                    for (var i = 0; i < Markers.length; i++) {
                        var tempMarker = Markers[i];
                        // Draw marker

                        // Calculate postion text
                        if (i == 0) {
                            context.drawImage(tempMarker.Sprite, tempMarker.XPos, tempMarker.YPos, tempMarker.Width, tempMarker.Height);
                            markerText = "Starting (X:" + Math.round(tempMarker.XPos) + ", Y:" + Math.round(tempMarker.YPos);
                        } else if (i == Markers.length - 1) {
                            context.drawImage(tempMarker.Sprite, tempMarker.XPos, tempMarker.YPos, tempMarker.Width, tempMarker.Height);
                            markerText = "Target (X:" + Math.round(tempMarker.XPos )+ ", Y:" + Math.round(tempMarker.YPos);
                        } else
                            markerText = "";

                        // Draw a simple box so you can see the position
                        var textMeasurements = context.measureText(markerText);
                        context.fillStyle = "#666";
                        context.globalAlpha = 0.7;
                        context.fillRect(tempMarker.XPos - (textMeasurements.width / 2), tempMarker.YPos - 15, textMeasurements.width, 20);
                        context.globalAlpha = 1;

                        // Draw position above
                        context.fillStyle = "#000";
                        context.fillText(markerText, tempMarker.XPos, tempMarker.YPos);
                    }
                };
                //draw();
                setInterval(draw, (1000 / 60)); // Refresh 60 times a second

        }
    }
</script>

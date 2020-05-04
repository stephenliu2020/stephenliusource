//pathandpoint.vue
<template>
    <div>
        <h2>{{title}}</h2>
        <hr>
        <!--button @click="loadmap()">Add Map</button-->
        <button @click="init()">Add Map</button>
        <canvas id="Canvas" width="700" height="700"></canvas>
    </div>
</template>

<script>
    export default{
        data(){
            return {
                title:"Path and Point",
                sContent:"This is task components"
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

            let  Marker = function () {
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
            let markDot = function (x,y) {
                let markerText = ".";
                // Draw position above
                context.fillStyle = "#000";
                context.fillText(markerText, x, y);
            }

            let line = function (x0,y0,x1,y1) {
                // Calculate "deltas" of the line (difference between two ending points)
                let dx = x1 - x0;
                let dy = y1 - y0;
                // Calculate the line equation based on deltas
                let D = (2 * dy) - dx;
                let y = y0;
                // Draw the line based on arguments provided
                for (let x = x0; x < x1; x++)
                {
                    // Place pixel on the raster display
                    markDot(x, y);
                    if (D >= 0)
                    {
                        y = y + 1;
                        D = D - 2 * dx;
                    }
                    D = D + 2 * dy;
                }
            }

            let drawLine = function(X1, Y1, X2, Y2){
               // context.save();
                context.beginPath();
               // context.translate(X1, Y1);
                context.moveTo(Math.round(X1), Math.round(Y1));
                context.lineTo(Math.round(X2), Math.round(Y2));
                context.stroke();
                //context.restore();
            }

            let draw = function () {
                // Clear Canvas
                context.fillStyle = "#000";
                context.fillRect(0, 0, canvas.width, canvas.height);

                // Draw map
                // Sprite, X location, Y location, Image width, Image height
                // You can leave the image height and width off, if you do it will draw the image at default size
                context.drawImage(mapSprite, 0, 0, 700, 700);

                // Draw markers
                for (var i = 0; i < Markers.length; i++) {
                    var tempMarker = Markers[i];
                    // Draw marker
                    if (i==0||i==Markers.length-1)
                        context.drawImage(tempMarker.Sprite, Math.round(tempMarker.XPos),Math.round( tempMarker.YPos), Math.round(tempMarker.Width), Math.round(tempMarker.Height));

                    // Calculate postion text
                   /* var markerText = "Postion (X:" + tempMarker.XPos + ", Y:" + tempMarker.YPos;
                    // Draw a simple box so you can see the position
                    var textMeasurements = context.measureText(markerText);
                    context.fillStyle = "#666";
                    context.globalAlpha = 0.7;
                    context.fillRect(tempMarker.XPos - (textMeasurements.width / 2), tempMarker.YPos - 15, textMeasurements.width, 20);
                    context.globalAlpha = 1;

                    // Draw position above
                    context.fillStyle = "#000";
                    context.fillText(markerText, tempMarker.XPos, tempMarker.YPos);*/
                    if (i!=Markers.length-1)
                        drawLine(tempMarker.XPos , tempMarker.YPos, Markers[i+1].XPos, Markers[i+1].YPos);
                }
            };
            //draw();
            setInterval(draw, (1000 / 60)); // Refresh 60 times a second
        },

    }
</script>

//speedsetting.vue
<template>
    <div>
        <h2>{{title}}</h2>
        <hr>
        <p>{{sContent}}</p>
        <h3>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../assets/route.png"> &nbsp;<br><el-button type="text" @click="onSpeed('/gs-robot/cmd/set_speed_level')">Motion Speed</el-button>
            <br><br><br><br>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../assets/route.png"> &nbsp;<br><el-button type="text" @click="onSpeed('/gs-robot/cmd/set_navigation_speed_level')">Navigation Speed</el-button>
        </h3>
        <p id="commandResponse"></p>
    </div>
</template>
<script>
    export default{
        data(){
            return {
                title:"Speed Settings",
                sContent:"This is speed setting components"
            }
        },
        methods:{
              onSpeed : function(str){
                  let method = "GET";
                  let shouldBeAsync = true;
                  let request = new XMLHttpRequest();

                  request.onload = function () {

                      // You can get all kinds of information about the HTTP response.
                      //var status = request.status; // HTTP response status, e.g., 200 for "200 OK"
                      let data = request.responseText; // Returned data, e.g., an HTML document.
                      document.getElementById("commandResponse").innerHTML = request.response;

                      alert(data);
                  }
                  let theUrl = "http://192.168.0.1:8080" ;
                  let  url = theUrl + str;
                  request.open(method, url, shouldBeAsync);

                  request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
                  request.send(null);
              }

            }
    }
</script>
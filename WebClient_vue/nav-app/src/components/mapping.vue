//mapping.vue
<template>
    <div id="map_canvas" style="height: 350px;width: 500px;">
        <h2>{{title}}</h2>
        <hr>
        <p>{{sContent}}</p>
        <button @click="init()">Add Map</button>
    </div>
</template>

<script>
    export default{
        data(){
            return {
                title:"Mapping",
                sContent:"This is mapping components",
                mapLoaded: false
            }
        },
        /*mounted() {
            let recaptchaScript = document.createElement('script')
            recaptchaScript.setAttribute('src', 'http://maps.google.com/maps/api/js?v=3.1&sensor=true')
            document.head.appendChild(recaptchaScript)
        },*/
        methods:{
            init:function() { //回调的方法
                var myOptions = {
                    zoom: 15,
                    center: new google.maps.LatLng(29.82579, 121.56413),
                    mapTypeId: google.maps.MapTypeId.ROADMAP
                };
                var map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);
                // 这里填上轨迹的经纬坐标，或者ajax从后台读取
                // 坐标做好是在路口的，不然画线不在路上。
                var trackPoints = [
                    new google.maps.LatLng(29.83092, 121.55923),
                    new google.maps.LatLng(29.83020, 121.55923),
                    new google.maps.LatLng(29.83015, 121.56059),
                    new google.maps.LatLng(29.82929, 121.56423),
                    new google.maps.LatLng(29.82276, 121.56407),
                    new google.maps.LatLng(29.82273, 121.56652),
                    new google.maps.LatLng(29.82234, 121.56652),
                    new google.maps.LatLng(29.82209, 121.56695),
                ];

                var trackPath = new google.maps.Polyline({
                    path: trackPoints,
                    strokeColor: "#FF0000", // 线条颜色
                    strokeOpacity: 1.0, // 线条透明度
                    strokeWeight: 2 // 线条粗细
                });

                trackPath.setMap(map);    //画线
            }

        },
        mounted(){
            let recaptchaScript = document.createElement('script');
            recaptchaScript.setAttribute('src', 'http://maps.google.com/maps/api/js?v=3.1&sensor=true');
            document.head.appendChild(recaptchaScript);
           // let initialize = function () {
                let myOptions = {
                    zoom: 15,
                    center: new google.maps.LatLng(29.82579, 121.56413),
                    mapTypeId: google.maps.MapTypeId.ROADMAP
                };
                let map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);
                // 这里填上轨迹的经纬坐标，或者ajax从后台读取
                // 坐标做好是在路口的，不然画线不在路上。
                let trackPoints = [
                    new google.maps.LatLng(29.83092, 121.55923),
                    new google.maps.LatLng(29.83020, 121.55923),
                    new google.maps.LatLng(29.83015, 121.56059),
                    new google.maps.LatLng(29.82929, 121.56423),
                    new google.maps.LatLng(29.82276, 121.56407),
                    new google.maps.LatLng(29.82273, 121.56652),
                    new google.maps.LatLng(29.82234, 121.56652),
                    new google.maps.LatLng(29.82209, 121.56695),
                ];

                let trackPath = new google.maps.Polyline({
                    path: trackPoints,
                    strokeColor: "#FF0000", // 线条颜色
                    strokeOpacity: 1.0, // 线条透明度
                    strokeWeight: 2 // 线条粗细
                });

                trackPath.setMap(map);    //画线
            //};
            //initialize();
        },
    }

</script>

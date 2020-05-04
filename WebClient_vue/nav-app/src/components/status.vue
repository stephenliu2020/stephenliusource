//status.vue
<template>
    <div>
        <h2>{{title}}</h2>
        <hr>
        <p>{{sContent}}</p>
        <h3>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../assets/robot_status_device.png"> &nbsp;&nbsp<router-link to='/status'><br>Device Status</router-link>
            <br><br><br><br>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../assets/robot_status_sensor.png"> &nbsp;&nbsp<router-link to='/status'><br>Sensor Status</router-link>
            <br><br><br><br>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../assets/robot_status_advanced.png"> &nbsp;&nbsp<router-link to='/status'><br>Advanced Status</router-link>
        </h3>
    </div>
</template>
<script>
    export default{
        data(){
            return {
                title:"Robot Status",
                sContent:"This is status components"
            }
        }
    }
</script>
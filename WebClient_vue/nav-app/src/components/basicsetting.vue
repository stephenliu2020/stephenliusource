//basicsetting.vue
<template>
    <div>
        <h2>{{title}}</h2>
        <hr>
        <p>{{sContent}}</p>
        <h3>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../assets/speed.png"> &nbsp;&nbsp<router-link to='/speedsetting'><br>Speed Setting</router-link>
            <br><br><br><br>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../assets/gaussian_setting_threshold.png"> &nbsp;&nbsp<router-link to='/basicsetting'><br>Function Switch</router-link>
            <br><br><br><br>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../assets/gaussian_setting_camera_reset.png"> &nbsp;&nbsp<router-link to='/basicsetting'><br>Camera Calibrate</router-link>
            <br><br><br><br>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../assets/gaussian_setting_screen_lock.png"> &nbsp;&nbsp<router-link to='/basicsetting'><br>Screen Lock</router-link>
        </h3>
    </div>
</template>
<script>
    export default{
        data(){
            return {
                title:"Basic Settings",
                sContent:"This is basic setting components"
            }
        }
    }
</script>
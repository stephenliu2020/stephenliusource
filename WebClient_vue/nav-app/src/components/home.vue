//home.vue
<template>
 <div>
     <h2>{{sContent}}</h2>
     <br><br><br><br>
     <h3>
         &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../assets/gaussian_main_input.png"> &nbsp;&nbsp<router-link to='/mapping'>Mapping</router-link>
         &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
         <img src="../assets/gaussian_path_point.png"> &nbsp;&nbsp<router-link to='/pathandpoint'>Path and Point</router-link>
         &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
         <img src="../assets/gaussian_main_executequeue.png"> &nbsp;&nbsp<router-link to='/taskqueue'>Task</router-link>
         &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
         &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../assets/gaussian_main_setup.png"> &nbsp;&nbsp<router-link to='/setting'>Settings</router-link>
         &nbsp;&nbsp;&nbsp;&nbsp;
         &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../assets/gaussian_main_robot_status.png"> &nbsp;&nbsp<router-link to='/status'>Robot Status</router-link>
     </h3>
     <hr>
     <br>
 </div>
</template>
<script>
 export default{
     data(){
         return {
             sContent:"Main Menu"
         }
     }
 }
</script>
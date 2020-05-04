//setting.vue
<template>
 <div>
     <h2>{{title}}</h2>
     <hr>
     <p>{{sContent}}</p>
     <h3>
         &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../assets/basic_setting.png"> &nbsp;&nbsp<router-link to='/basicsetting'><br>Basic Setting</router-link>
         <br><br><br><br>
         &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../assets/advanced_setting.png"> &nbsp;&nbsp<router-link to='/setting'><br>Advanced Setting</router-link>
         <br><br><br><br>
         &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../assets/robot_data.png"> &nbsp;&nbsp<router-link to='/setting'><br>Data Operation</router-link>
     </h3>
 </div>
</template>
<script>
 export default{
     data(){
         return {
             title:"Settings",
             sContent:"This is setting components"
         }
     }
 }
</script>
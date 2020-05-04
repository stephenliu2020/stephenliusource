<template>
  <div id="app">
    <div>
      <router-link to='/home'><img src="./assets/logo_esi.png"></router-link>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
      &nbsp;&nbsp;&nbsp;&nbsp;<router-link to='/login'><img src="./assets/gaussian_wifi_middle.png"></router-link>
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Auto Nav&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="./assets/gaussian_title_warn.png">
      &nbsp;&nbsp;<img src="./assets/gaussian_title_battery2.png">60% <br>

    </div>
    <!--Login msg="Welcome to Your Vue.js App"/-->
	<router-view></router-view>
  </div>
</template>

<script src="https://unpkg.com/vue/dist/vue.js"></script>
<script src="https://unpkg.com/vue-router/dist/vue-router.js"></script>

<script>
import login from './components/login.vue'
import home from './components/home.vue'

export default {
  name: 'app',
  components: {
    login,
    home
  }
}
</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>

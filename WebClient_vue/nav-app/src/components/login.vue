<template>
  <div class="firstdemo">
      <el-form ref="form" :model="form" label-width="100px">
          <el-row type="flex" justify="center">
              <el-col :span="5">
                  <el-form-item label-width="70px">
                    <span><font color="pink" size="3">Register and Login</font></span>
                  </el-form-item>
              </el-col>
          </el-row>
          <el-row type="flex" justify="center">
              <el-col :span="5">
                  <el-form-item label="Username:" prop="name">
                      <el-input v-model="form.name" size="small"></el-input>
                  </el-form-item>
              </el-col>
          </el-row>
          <el-row type="flex" justify="center">
              <el-col :span="5">
                  <el-form-item label="Password:" prop="password">
                      <el-input v-model="form.password" size="small"></el-input>
                  </el-form-item>
              </el-col>
          </el-row>
          <el-row type="flex" justify="center">
              <el-col :span="5">
                  <el-form-item>
                      <el-button type="primary" @click="submit">Login</el-button>
                      <el-button @click="register">Register</el-button>
                  </el-form-item>
              </el-col>
          </el-row>
      </el-form>

  </div>
</template>
 
 

<script>
import './home.vue'
export default {
  name: 'Login',

  data () {
    return {
        form:{
            name:'',
            password:'',
        },
      
    }
  },
  methods:{
      jumpMain:function(){
          this.$router.push({name:'home'})
      },
      submit:function(){
          if(this.form.name==="admin"&&this.form.password==="123456")
          {
            //alert("Login success")
            //document.body.innerHTML = "";
			this.text = "";
            this.$router.push({path:'home'})
          }
          else
          alert("Login failed")
      },
      register:function(){},

  },
}
</script>
 

<style scoped>
 
</style>
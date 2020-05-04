<template>
	<div>
		<div>
			搜索区
		</div>
		<div class="white-bg margin auto">
			<el-table>

				<el-table-column prop="updTime" label="任务实际耗时(S)" width="180"  align="center"></el-table-column>
				<el-table-column prop="status" label="任务状态" :formatter="getTaskStatus" width="100" align="center"></el-table-column>
				<el-table-column label="操作" width="194" align="center">
              	<template slot-scope="scope">
              		<el-button type="text" size="small" @click="lookUserInfo(scope.row)" icon="iconfont icon-chakan color-bule"> 查看</el-button>         		     
              	</template>
			</el-table>
		</div>
		<user-manager  v-if="userShow" v-bind:userData = "userData" @close="showUserInfo" > </user-manager>	
	</div>
</template>
<script>
	//导入用户页面
	import userManager from '../userManager.vue';
	export default {
	 	name:'taskManager',
	 	components:{userManager},
	 	data() {
	 		return {
				userData:{
					userShow:false,
					userList:[],
					//table展示数据略
				}
				
			}
		},
		methods: {
			lookTaskInfo(row) {
				this.userShow = true;
				//this.userList = 查询出来的数据（或者是你想要传的数据）
			},
			//此方法，子组件会调用，调用返回父页面
			showUserInfo() {
				this.userShow = false;
			},
			//请求数据方法
			getTableData() {
				//请求数据接口略
			}
		}
	}	
</script>
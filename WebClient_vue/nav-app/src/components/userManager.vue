<template>
	<div>
		<!-- 返回父页面按钮 -->
		<div v-if="!isShowUserData" class="margin15 margin-top10">
            <el-button type="warning" size="mini" icon="iconfont icon-iconfont0104" @click="goToTaskList">   返回</el-button>
        </div>
		<div v-if="isShowUserData">
			搜索区
		</div>
		<div class="white-bg margin auto">
			<el-table>
				数据表格展示区
				<el-table-column prop="updTime" label="任务实际耗时(S)" width="180"  align="center"></el-table-column>
				<el-table-column prop="status" label="任务状态" :formatter="getTaskStatus" width="100" align="center"></el-table-column>
				<el-table-column label="操作" width="194" align="center">
              	<template slot-scope="scope">
              		<el-button type="text" size="small" @click="lookUserInfo(scope.row)" icon="iconfont icon-chakan color-bule"> 查看</el-button>         		     
              	</template>
			</el-table>
		</div>
	</div>
</template>
<script>
	export default {
	 	name:'userManager',
		props: ['userData'],  //接收父组件的参数
	 	data() {
	 		return {
				isShowUserData:true, //是否是从任务页面而来
				table展示数据略
			}
		},
		mounted: function () {
			if(!this.userData.isShowUserData){
				this.isShowUserData = this.userData.isShowUserData;
			}
        },
		methods: {
			goToTaskList() {
				//调父页面方法
			    this.$emit('close');
				//调父页面请求数据方法
				this.$parent.getTableData();
			}
		}
	}	
</script>
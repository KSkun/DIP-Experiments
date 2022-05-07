<template>
  <div id="app">
    <el-row :gutter="20">
      <el-col :span="8">
        <el-card :body-style="{ padding: '0px' }" class="image-card">
          <div class="image-area"></div>
          <div style="padding: 14px;">原图</div>
        </el-card>
      </el-col>
      <el-col :span="8">
        <el-card :body-style="{ padding: '0px' }">
          <div class="image-area"></div>
          <div style="padding: 14px;">高斯滤波</div>
        </el-card>
      </el-col>
      <el-col :span="8">
        <el-card :body-style="{ padding: '0px' }">
          <div class="image-area"></div>
          <div style="padding: 14px;">梯度</div>
        </el-card>
      </el-col>
    </el-row>
    <el-row :gutter="20">
      <el-col :span="8">
        <el-card :body-style="{ padding: '0px' }">
          <div class="image-area"></div>
          <div style="padding: 14px;">非极大值抑制</div>
        </el-card>
      </el-col>
      <el-col :span="8">
        <el-card :body-style="{ padding: '0px' }">
          <div class="image-area"></div>
          <div style="padding: 14px;">双阈值</div>
        </el-card>
      </el-col>
      <el-col :span="8">
        <el-card :body-style="{ padding: '0px' }">
          <div class="image-area"></div>
          <div style="padding: 14px;">结果</div>
        </el-card>
      </el-col>
    </el-row>
    <el-card>
      <el-form ref="form" label-width="150px" size="mini">
        <el-form-item>
          <span slot="label">原始图片</span>
          <el-button type="primary" size="mini" v-on:click="onSelectImage()">选择</el-button>
          <p>{{ imageFile }}</p>
        </el-form-item>
        <el-form-item>
          <span slot="label">高斯函数标准差 <vue-mathjax formula="$\sigma$"></vue-mathjax></span>
          <el-input-number v-model="sigma" :precision="3" :step="0.01" :min="0" :max="10"></el-input-number>
        </el-form-item>
        <el-form-item>
          <span slot="label">低阈值 <vue-mathjax formula="$T_L$"></vue-mathjax></span>
          <el-input-number v-model="thrLow" :precision="3" :step="0.01" :min="0" :max="1"></el-input-number>
        </el-form-item>
        <el-form-item>
          <span slot="label">高阈值 <vue-mathjax formula="$T_H$"></vue-mathjax></span>
          <el-input-number v-model="thrHigh" :precision="3" :step="0.01" :min="0" :max="1"></el-input-number>
        </el-form-item>
        <el-form-item>
          <el-button type="success" size="mini">执行</el-button>
          <el-button type="primary" size="mini">保存结果</el-button>
        </el-form-item>
      </el-form>
    </el-card>
  </div>
</template>

<script>
const {dialog} = require('electron').remote
const canny = require('electron').remote.require('../../build/Release/nodecanny')

export default {
  name: 'electron-canny',
  data() {
    return {
      sigma: 4,
      thrLow: 0.04,
      thrHigh: 0.10,
      imageFile: ''
    }
  },
  methods: {
    onSelectImage: function () {
      dialog.showOpenDialog({
        properties: ['openFile'],
        filters: [
          {name: '图片', extensions: ['jpg', 'jpeg', 'png', 'bmp']}
        ]
      }).then(result => {
        if (result.canceled) return
        var path = result.filePaths[0]
        console.log('select image ' + path)
        this.$data.imageFile = path
        canny.loadImage(path)
      })
    }
  }
}
</script>

<style>
#app {
  margin: 30px;
}

.el-row {
  margin-bottom: 20px;
}

.image-area {
  height: 150px;
  overflow: hidden;
  background-image: url("/static/avatar-kiana.jpg");
  background-repeat: no-repeat;
  background-size: contain;
  background-position: center;
}
</style>

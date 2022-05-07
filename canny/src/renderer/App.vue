<template>
  <div id="app">
    <el-row :gutter="20">
      <el-col :span="8">
        <el-card :body-style="{ padding: '0px' }" class="image-card">
          <div class="image-area" :style="{'background-image': 'url(' + imageOrigin + ')'}"></div>
          <div style="padding: 14px;">原图</div>
        </el-card>
      </el-col>
      <el-col :span="8">
        <el-card :body-style="{ padding: '0px' }">
          <div class="image-area" :style="{'background-image': 'url(' + imageFiltered + ')'}"></div>
          <div style="padding: 14px;">高斯滤波</div>
        </el-card>
      </el-col>
      <el-col :span="8">
        <el-card :body-style="{ padding: '0px' }">
          <div class="image-area" :style="{'background-image': 'url(' + imageGradient + ')'}"></div>
          <div style="padding: 14px;">梯度</div>
        </el-card>
      </el-col>
    </el-row>
    <el-row :gutter="20">
      <el-col :span="8">
        <el-card :body-style="{ padding: '0px' }">
          <div class="image-area" :style="{'background-image': 'url(' + imageSuppressed + ')'}"></div>
          <div style="padding: 14px;">非极大值抑制</div>
        </el-card>
      </el-col>
      <el-col :span="8">
        <el-card :body-style="{ padding: '0px' }">
          <div class="image-area" :style="{'background-image': 'url(' + imageThreshold + ')'}"></div>
          <div style="padding: 14px;">双阈值</div>
        </el-card>
      </el-col>
      <el-col :span="8">
        <el-card :body-style="{ padding: '0px' }">
          <div class="image-area" :style="{'background-image': 'url(' + imageResult + ')'}"></div>
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
          <span slot="label">梯度算子</span>
          <el-radio-group v-model="gradKern" size="mini">
            <el-radio-button :label="1">罗伯特</el-radio-button>
            <el-radio-button :label="2">Prewitt</el-radio-button>
            <el-radio-button :label="3">Sobel</el-radio-button>
          </el-radio-group>
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
          <el-button type="success" size="mini" v-on:click="onExecute()">执行</el-button>
          <el-button type="primary" size="mini" v-on:click="onSaveResults()">保存结果</el-button>
        </el-form-item>
      </el-form>
    </el-card>
  </div>
</template>

<script>
const {dialog} = require('electron').remote
const canny = require('electron').remote.require('../../nodecanny')

export default {
  name: 'electron-canny',
  data() {
    return {
      sigma: 4,
      gradKern: 1,
      thrLow: 0.04,
      thrHigh: 0.10,
      imageFile: '',
      imageOrigin: '',
      imageFiltered: '',
      imageGradient: '',
      imageSuppressed: '',
      imageThreshold: '',
      imageResult: ''
    }
  },
  methods: {
    onSelectImage: function () {
      dialog.showOpenDialog({
        title: '打开要处理的图片',
        properties: ['openFile'],
        filters: [
          {name: '图片', extensions: ['jpg', 'jpeg', 'png', 'bmp']}
        ]
      }).then(result => {
        if (result.canceled) return
        const path = result.filePaths[0];
        console.log('select image ' + path)
        if (!canny.loadImage(path)) return
        this.$data.imageFile = path
        this.$data.imageOrigin = canny.getImage(0)
        this.$data.imageFiltered = ''
        this.$data.imageGradient = ''
        this.$data.imageSuppressed = ''
        this.$data.imageThreshold = ''
        this.$data.imageResult = ''
      })
    },
    onExecute: function () {
      if (this.$data.imageFile === '') return
      canny.doProcess(this.$data.sigma, this.$data.gradKern, this.$data.thrLow, this.$data.thrHigh)
      this.$data.imageFiltered = canny.getImage(1)
      this.$data.imageGradient = canny.getImage(2)
      this.$data.imageSuppressed = canny.getImage(3)
      this.$data.imageThreshold = canny.getImage(4)
      this.$data.imageResult = canny.getImage(5)
    },
    savePNGToFile: function (png, path) {
      png = png.replace(/^data:image\/png;base64,/, "")
      let buffer = new Buffer(png, 'base64').toString('binary')
      require('fs').writeFile(path, buffer, 'binary', function (err) {
        if (err) console.log(err)
      })
    },
    onSaveResults: function () {
      if (this.$data.imageResult === '') return
      dialog.showOpenDialog({
        title: '选择保存目录',
        properties: ['openDirectory', 'createDirectory']
      }).then(result => {
        if (result.canceled) return
        const saveDir = result.filePaths[0];
        console.log('save image at ' + saveDir)
        let oriFilename = require('path').basename(this.$data.imageFile)
        oriFilename = oriFilename.replace('\.', '_')
        this.savePNGToFile(this.$data.imageFiltered, saveDir + '/' + oriFilename + '_filtered.png')
        this.savePNGToFile(this.$data.imageGradient, saveDir + '/' + oriFilename + '_gradient.png')
        this.savePNGToFile(this.$data.imageSuppressed, saveDir + '/' + oriFilename + '_suppressed.png')
        this.savePNGToFile(this.$data.imageThreshold, saveDir + '/' + oriFilename + '_threshold.png')
        this.savePNGToFile(this.$data.imageResult, saveDir + '/' + oriFilename + '_result.png')
        console.log('save as ' + saveDir + '/' + oriFilename + '_result.png')
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
  background-repeat: no-repeat;
  background-size: contain;
  background-position: center;
}
</style>

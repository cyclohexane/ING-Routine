> 转义=原样输出文本

## 常用指令：
- {{}} v-text="xxx" v-html="xxx"//插值（单向绑定）
- v-if="xxx" v-else-if="xxx" v-else v-show="xxx" //是否显示元素
- v-model="xxx" //可以在input中使用（双向绑定）
- v-on:xxx="xxx" //简写为@xxx="xxx"，事件监听
- v-bind:xxx="xxx" //简写为:xxx="xxx"，添加html属性（单向绑定）

## 特殊用法：
- 参数：在指令后以冒号指明
- 修饰符: <form v-on:submit.prevent="onSubmit">
- JS表达式：
  - <div v-bind:id="'list-' + id">
  - {{ ok ? 'YES' : 'NO' }}
  - {{ message.split('').reverse().join('') }}
- 过滤器（实现文本格式化）：http://www.runoob.com/vue2/vue-template-syntax.html
- 可以在一个标签内写多个v-bind等。

### 备注：
- <a href="javascript:void(0)" v-on:click="tap">点我</a>
  //a要禁止刷新，否则vue会渲染失败

### Vue组件的相互调用：

- 被调用组件：
name: 'HelloWorld'

- 当前组件：
import HelloWorld from './components/HelloWorld'
在compoments里添加HelloWorld
然后在<template>中合适的位置插入<hello-world></hello-world>，因为如果单词中有大写，会被判断为多个单词，用-拆分开来（并转换为全小写）。
或者也可以插入<router-view/>，并编写路由文件。

如果data里出现了不存在的数据名，那么vue就炸了，啥都不会编译，但是浏览器开发者工具会出现错误提示。
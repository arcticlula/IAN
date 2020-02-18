<template>
  <b-col cols="12" md="4">
    <b-card>
      <b-tabs content-class="mt-3">
        <b-tab title="Patterns" active variant="info">
          <b-row class="mb-2" style="text-align: left;">
            <b-col cols="12">
              <b-button
                v-bind:class="{ active: patterns.shape == 'circle'}"
                @click="setShape('circle')"
                size="sm"
                class="mr-1"
                variant="info"
              >
                <b-icon icon="circleHalf"></b-icon>
              </b-button>
              <b-button
                v-bind:class="{ active: patterns.shape == 'square'}"
                @click="setShape('square')"
                size="sm"
                class="mr-1"
                variant="info"
              >
                <b-icon icon="squareHalf"></b-icon>
              </b-button>
              <b-button
                v-bind:class="{ active: patterns.shape == 'triangle'}"
                @click="setShape('triangle')"
                size="sm"
                class="mr-1"
                variant="info"
              >
                <b-icon icon="triangleHalf"></b-icon>
              </b-button>
              <b-button
                v-bind:class="{ active: patterns.shape == 'star'}"
                @click="setShape('star')"
                size="sm"
                variant="info"
              >
                <b-icon icon="starHalf"></b-icon>
              </b-button>
            </b-col>
          </b-row>
          <b-row class="mb-2">
            <b-col cols="8" class="pr-0">
              <b-form-select
                @change="setShapeDraw"
                v-model="patterns.selected"
                :options="currOptionShape"
                size="sm"
              ></b-form-select>
            </b-col>
            <b-col cols="4">
              <b-form-select
                @change="canvasObj.draw()"
                v-model="patterns.div"
                :options="currOptionDiv"
                size="sm"
              >
                <!-- <option v-for="opt in currOptionDiv" :value="opt" :key="opt">{{opt}}</option> -->
              </b-form-select>
            </b-col>
          </b-row>
          <b-row class="mb-2">
            <b-col cols="12">
              <b-form-select size="sm"></b-form-select>
            </b-col>
          </b-row>
          <b-row class="mb-2" style="text-align: left;">
            <b-col cols="12">
              <b-button
                v-bind:class="{ active: patterns.orientation == 0}"
                @click="setOrientation(0)"
                size="sm"
                class="mr-1"
                variant="info"
              >
                <b-icon icon="arrowsAngleExpand"></b-icon>
              </b-button>
              <b-button
                v-bind:class="{ active: patterns.shape == 1}"
                @click="setOrientation(1)"
                size="sm"
                class="mr-1"
                variant="info"
              >
                <b-icon icon="arrowsAngleContract"></b-icon>
              </b-button>
            </b-col>
          </b-row>
          <b-row class="mb-2">
            <b-col
              cols="12"
              style="border-radius: 5px; width: 100%; margin: 0 auto; text-align: center;"
            >
              <canvas id="shapeCanvas" style="width: 100%;"></canvas>
            </b-col>
          </b-row>
          <b-row>
            <b-col cols="12">
              <b-form-checkbox
                @change="setPixel"
                v-model="patterns.pixel"
                switch
                value="circle"
                unchecked-value="square"
                size="sm"
              >Pixels</b-form-checkbox>
            </b-col>
          </b-row>
          <b-row style="text-align: right;">
            <b-col cols="12">
              <b-button @click="sendToDevice" style="background: #3793a0;" size="sm">
                <b-icon icon="upload"></b-icon>
              </b-button>
            </b-col>
          </b-row>
        </b-tab>
        <b-tab title="Text" color="info">
          <b-row class="mb-2">
            <b-col cols="12">
              <b-form-select size="sm"></b-form-select>
            </b-col>
          </b-row>
          <b-row style="text-align: left;">
            <b-col cols="12">
              <b-form-checkbox switch size="sm">Centrado</b-form-checkbox>
            </b-col>
          </b-row>
        </b-tab>
        <b-tab title="Colors" color="info">
          <swatches row-length="5" colors="basic" v-model="color"></swatches>
          <swatches row-length="5" colors="basic" v-model="color"></swatches>
          <swatches row-length="5" colors="basic" v-model="color"></swatches>
        </b-tab>
      </b-tabs>
    </b-card>
  </b-col>
</template>

<script>
// @ is an alias to /src
import { mapState, mapMutations, mapGetters } from "vuex";
import canvasShapes from "@/components/canvas.js";
import hdlc from "@/components/hdlc.js";
import Swatches from "vue-swatches";
import "vue-swatches/dist/vue-swatches.min.css";
// import HelloWorld from "@/components/HelloWorld.vue";

export default {
  name: "Home",
  // components: {
  //   HelloWorld
  // },
  data: function() {
    return {
      currOptionShape: [],
      currOptionDiv: [],
      canvasObj: {},
      color: "",
      enc: null
    };
  },
  components: { Swatches },
  computed: {
    ...mapState(["bluetooth", "patterns", "optionsShape", "optionsDiv"])
  },
  methods: {
    // ...mapMutations(["setDevicesSaved", "setDevicesUnpaired", "setState"]),
    sendCommBluetooth(message) {
      let msg = this.enc.encode(message);
      console.log(msg);
      this.hdlc.minihdlc_send_frame(msg, msg.length);
    },
    setPixel(px) {
      this.patterns.pixel = px;
      this.canvasObj.draw();
    },
    blueSend(data) {
      // let hex = Number(data).toString(16);
      // if (hex.length < 2) {
      //   hex = "0" + hex;
      // }

      // console.log(data, hex);
      let element = new Uint8Array([data]);
      bluetoothSerial.write(element, this.success, this.failure);
    },
    blueProcess(data, length) {
      for (let i = 0; i < length; i++) {
        console.log(String.fromCharCode(data[i]));
      }
    },
    sendToDevice() {
      let msg = [];
      console.log();
      this.hdlc.minihdlc_send_frame(msg, msg.length);
    },
    setShape(shape) {
      this.patterns.shape = shape;
      this.currOptionShape = this.optionsShape[shape];
      this.patterns.selected = this.currOptionShape[0];
      console.log(this.patterns, this.currOptionShape);
      this.currOptionDiv = this.optionsDiv[shape][this.patterns.selected];
      this.canvasObj.draw();
    },
    setShapeDraw() {
      console.log(this.optionsDiv[this.patterns.shape], this.patterns.selected);
      this.currOptionDiv = this.optionsDiv[this.patterns.shape][
        this.patterns.selected
      ];
      this.patterns.div = this.currOptionDiv[0];
      // console.log(this.currOptionDiv);
      this.canvasObj.draw();
    },
    setOrientation(orientation) {
      this.patterns.orientation = orientation;
      this.canvasObj.draw();
    },
    success() {
      console.log("sim");
    },
    failure() {
      console.log("nao");
    }
  },
  mounted() {
    let scope = this;
    // this.enc = new TextEncoder(); // always utf-8
    this.currOptionShape = this.optionsShape[this.patterns.shape];
    this.patterns.selected = this.currOptionShape[0].value;
    this.currOptionDiv = this.optionsDiv[this.patterns.shape][
      this.patterns.selected
    ];
    let canvas = document.getElementById("shapeCanvas");
    let ctx = canvas.getContext("2d");
    this.canvasObj = new canvasShapes(canvas, ctx);
    this.canvasObj.draw();
    this.hdlc = new hdlc();
    this.hdlc.minihdlc_init(this.blueSend, this.blueProcess);
    bluetoothSerial.subscribeRawData(
      function(data) {
        // console.log("hey");
        let bytes = new Uint8Array(data);
        for (let i = 0; i < bytes.length; i++) {
          const element = bytes[i];
          scope.hdlc.minihdlc_char_receiver(element);
        }
      },
      function() {
        console.log("nope");
      }
    );
  }
};
</script>
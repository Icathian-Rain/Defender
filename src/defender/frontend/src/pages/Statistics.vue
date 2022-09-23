<template>
    <div class="w-full h-full">
        <div class="w-full text-center mb-5">
            <span class="text-xl"> 总操作数: {{ totalNum }} </span>
        </div>
        <div class="w-1/2 h-4/5 inline-block">
            <div id="barchart" class="w-full h-full"></div>
        </div>
        <div class="w-1/2 h-4/5 inline-block">
            <div id="piechart" class="w-full h-full"></div>
        </div>
    </div>
</template>

<script setup>
import { ref } from "vue";
import { EventsOn } from "../../wailsjs/runtime/runtime";
import { GetStatistics } from "../../wailsjs/go/main/App";
import { onMounted } from "vue";

// echarts
import * as echarts from "echarts/core";
import {
    GridComponent,
    TitleComponent,
    TooltipComponent,
    LegendComponent,
} from "echarts/components";
import { BarChart, PieChart } from "echarts/charts";
import { CanvasRenderer } from "echarts/renderers";
import { LabelLayout } from "echarts/features";

echarts.use([
    GridComponent,
    BarChart,
    CanvasRenderer,
    TitleComponent,
    TooltipComponent,
    LegendComponent,
    PieChart,
    LabelLayout,
]);

var barDom, PieDom;
var myBarChart, myPieChart;
var barOption, pieOption;
let totalNum = ref(0);

barOption = {
    title: {
        text: "操作风险分布",
        left: "center",
    },
    xAxis: {
        type: "category",
        data: ["低风险操作", "中风险操作", "高风险操作"],
    },
    yAxis: {
        type: "value",
    },
    series: [
        {
            data: [
                {
                    value: 0,
                    itemStyle: {
                        color: "#67c23a",
                    },
                },
                {
                    value: 0,
                    itemStyle: {
                        color: "#e6a23c",
                    },
                },
                {
                    value: 0,
                    itemStyle: {
                        color: "#f56c6c",
                    },
                },
            ],
            type: "bar",
            itemStyle: {
                normal:{
                    label: {
                    show: true, //开启显示
                    position: "top", //在上方显示
                    textStyle: {
                        //数值样式
                        color: "black",
                        fontSize: 16,
                    },
                },
                }
            },
        },
    ],
};

pieOption = {
    title: {
        text: "操作类型分布",
        left: "center",
    },
    tooltip: {
        trigger: "item",
    },
    legend: {
        orient: "vertical",
        left: "left",
    },
    series: [
        {
            name: "Operation Form",
            type: "pie",
            radius: "50%",
            data: [
                { value: 0, name: "弹窗操作" },
                { value: 735, name: "文件操作" },
                { value: 580, name: "堆操作" },
                { value: 484, name: "注册表操作" },
                { value: 300, name: "网络操作" },
            ],
            emphasis: {
                itemStyle: {
                    shadowBlur: 10,
                    shadowOffsetX: 0,
                    shadowColor: "rgba(0, 0, 0, 0.5)",
                },
            },
        },
    ],
};

onMounted(() => {
    barDom = document.getElementById("barchart");
    myBarChart = echarts.init(barDom);
    barOption && myBarChart.setOption(barOption);
    PieDom = document.getElementById("piechart");
    myPieChart = echarts.init(PieDom);
    pieOption && myPieChart.setOption(pieOption);
});

// 刷新统计数据
let reFresh = () => {
    GetStatistics().then((data) => {
        // console.log(data);
        totalNum.value = data["totalNum"];
        barOption.series[0].data[0].value = data["successNum"];
        barOption.series[0].data[1].value = data["warningNum"];
        barOption.series[0].data[2].value = data["errorNum"];
        myBarChart.setOption(barOption);
        pieOption.series[0].data[0].value = data["messageboxNum"];
        pieOption.series[0].data[1].value = data["fileNum"];
        pieOption.series[0].data[2].value = data["heapNum"];
        pieOption.series[0].data[3].value = data["regNum"];
        pieOption.series[0].data[4].value = data["netNum"];
        myPieChart.setOption(pieOption);
    });
};

// 监听UDP消息
// EventsOn("UDPMessage", () => {
//     reFresh();
// });

setInterval(() => {
    reFresh();
}, 1000);

reFresh();
</script>
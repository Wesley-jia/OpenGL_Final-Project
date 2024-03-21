import define1 from "./f780586a85a07fa7@363.js";

function _1(md,tex){return(
md`#  CSE6242_Spring2024_T5`
)}

function _side(md){return(
md`# 
## Demo page, Waiting for uploading the final project files
## ::Wesley ,2024/3/20
`
)}

function _3(md){return(
md`### Deaths from Alcohol Use Disorders, 1980`
)}

function _4(map,color,deaths,names,format){return(
map(
  id => color(deaths.get(id)[0]),
  id => `${names.get(id)}
${format(deaths.get(id)[0])} deaths per 100,000 people`
)
)}

function _5(legend,color){return(
legend(color, "Deaths per 100,000 people")
)}

function _6(md){return(
md`### Deaths from Alcohol Use Disorders, 2014`
)}

function _7(map,color,deaths,names,format,visibility){return(
map(
  id => color(deaths.get(id)[1]),
  id => `${names.get(id)}
${format(deaths.get(id)[1])} deaths per 100,000 people`,
  visibility
)
)}

function _8(md,tex,html,color){return(
md`t2`
)}

function _difference(md,tex){return(
md`## T3`
)}

function _10(md){return(
md`### Change in Deaths from Alcohol Use Disorders, 1980–2014`
)}

function _11(map,color2,deaths,names,formatChange,format,visibility){return(
map(
  id => color2(deaths.get(id)[1] - deaths.get(id)[0]),
  id => `${names.get(id)}
${formatChange(deaths.get(id)[1] - deaths.get(id)[0])} deaths per 100,000 people
${format(deaths.get(id)[0])} per 100,000 in 1980
${format(deaths.get(id)[1])} per 100,000 in 2014`,
  visibility
)
)}

function _12(legend,color2){return(
legend(color2, "Change in deaths per 100,000 people", "+d")
)}

function _13(md,tex,html,color2){return(
md`*T4`
)}

function _relative(md,tex){return(
md`## T5.`
)}

function _15(md){return(
md`### Change in Deaths from Alcohol Use Disorders, 1980–2014`
)}

function _16(map,color3a,deaths,names,formatPercentChange,format,visibility){return(
map(
  id => color3a((deaths.get(id)[1] - deaths.get(id)[0]) / deaths.get(id)[0]),
  id => `${names.get(id)}
${formatPercentChange((deaths.get(id)[1] - deaths.get(id)[0]) / deaths.get(id)[0])}
${format(deaths.get(id)[0])} per 100,000 in 1980
${format(deaths.get(id)[1])} per 100,000 in 2014`,
  visibility
)
)}

function _17(d3,legend,color3a){return(
d3.select(legend(color3a, "Percentage change of deaths per 100,000 people"))
  .call(g => g.selectAll(".tick text").text(d => (d > 0 ? "+" : "") + d * 100))
  .node()
)}

function _18(md,tex,html,color3a){return(
md`T6`
)}

function _19(md){return(
md`### Change in Deaths from Alcohol Use Disorders, 1980–2014`
)}

function _20(map,color3b,deaths,names,formatPercentChange,format,visibility){return(
map(
  id => color3b((deaths.get(id)[1] - deaths.get(id)[0]) / deaths.get(id)[0]),
  id => `${names.get(id)}
${formatPercentChange((deaths.get(id)[1] - deaths.get(id)[0]) / deaths.get(id)[0])}
${format(deaths.get(id)[0])} per 100,000 in 1980
${format(deaths.get(id)[1])} per 100,000 in 2014`,
  visibility
)
)}

function _21(d3,legend,color3b){return(
d3.select(legend(color3b, "Percentage change of deaths per 100,000 people"))
  .call(g => g.selectAll(".tick text").text(d => (d > 0 ? "+" : "") + d * 100))
  .node()
)}

function _22(md,tex,html,color3b){return(
md`T7`
)}

function _ratio(md,tex){return(
md`## T8`
)}

function _24(md){return(
md`### Change in Deaths from Alcohol Use Disorders, 1980–2014`
)}

function _25(map,color4,deaths,names,formatRatio,format,visibility){return(
map(
  id => color4(deaths.get(id)[1] / deaths.get(id)[0]),
  id => `${names.get(id)}
${formatRatio(deaths.get(id)[1] / deaths.get(id)[0])}
${format(deaths.get(id)[0])} per 100,000 in 1980
${format(deaths.get(id)[1])} per 100,000 in 2014`,
  visibility
)
)}

function _26(d3,legend,color4,formatRatio){return(
d3.select(legend(color4, "Relative likelihood of death", formatRatio))
  .node()
)}

function _27(md,tex,html,color4){return(
md`T9`
)}

function _28(md){return(
md`## T?
`
)}

function _29(md){return(
md`#### 

`
)}

function _30(md){return(
md`---

## Appendix`
)}

function _formatPercentChange(d3){return(
d3.format("+.1%")
)}

function _formatRatio(d3)
{
  const format = d3.format(".2~r");
  return x => format(x) + "×";
}


function _color3a(deaths,d3)
{
  const values = [...deaths.values()];
  return d3.scaleLinear()
      .domain([d3.min(values, ([a, b]) => (b - a) / a), 0, d3.max(values, ([a, b]) => (b - a) / a)])
      .range([-1, 0, 1])
      .interpolate((a, b) => a < 0 
          ? t => d3.interpolateBlues(1 - t) 
          : t => d3.interpolateReds(t));
}


function _color3b(deaths,d3)
{
  const values = [...deaths.values()];
  const max = Math.max(-d3.min(values, ([a, b]) => (b - a) / a), d3.max(values, ([a, b]) => (b - a) / a));
  return d3.scaleLinear()
      .domain([-max, 0, max])
      .range([-1, 0, 1])
      .interpolate((a, b) => a < 0 
          ? t => d3.interpolateBlues(1 - t) 
          : t => d3.interpolateReds(t));
}


function _color4(deaths,d3)
{
  const values = [...deaths.values()];
  const max = Math.max(d3.max(values, ([a, b]) => a / b), d3.max(values, ([a, b]) => b / a));
  return d3.scaleLog()
      .domain([1 / max, 1, max])
      .range([-1, 0, 1])
      .interpolate((a, b) => a < 0 
          ? t => d3.interpolateBlues(1 - t)
          : t => d3.interpolateReds(t));
}


export default function define(runtime, observer) {
  const main = runtime.module();
  main.variable(observer()).define(["md","tex"], _1);
  main.variable(observer("side")).define("side", ["md"], _side);
  main.variable(observer()).define(["md"], _3);
  main.variable(observer()).define(["map","color","deaths","names","format"], _4);
  main.variable(observer()).define(["legend","color"], _5);
  main.variable(observer()).define(["md"], _6);
  main.variable(observer()).define(["map","color","deaths","names","format","visibility"], _7);
  main.variable(observer()).define(["md","tex","html","color"], _8);
  main.variable(observer("difference")).define("difference", ["md","tex"], _difference);
  main.variable(observer()).define(["md"], _10);
  main.variable(observer()).define(["map","color2","deaths","names","formatChange","format","visibility"], _11);
  main.variable(observer()).define(["legend","color2"], _12);
  main.variable(observer()).define(["md","tex","html","color2"], _13);
  main.variable(observer("relative")).define("relative", ["md","tex"], _relative);
  main.variable(observer()).define(["md"], _15);
  main.variable(observer()).define(["map","color3a","deaths","names","formatPercentChange","format","visibility"], _16);
  main.variable(observer()).define(["d3","legend","color3a"], _17);
  main.variable(observer()).define(["md","tex","html","color3a"], _18);
  main.variable(observer()).define(["md"], _19);
  main.variable(observer()).define(["map","color3b","deaths","names","formatPercentChange","format","visibility"], _20);
  main.variable(observer()).define(["d3","legend","color3b"], _21);
  main.variable(observer()).define(["md","tex","html","color3b"], _22);
  main.variable(observer("ratio")).define("ratio", ["md","tex"], _ratio);
  main.variable(observer()).define(["md"], _24);
  main.variable(observer()).define(["map","color4","deaths","names","formatRatio","format","visibility"], _25);
  main.variable(observer()).define(["d3","legend","color4","formatRatio"], _26);
  main.variable(observer()).define(["md","tex","html","color4"], _27);
  main.variable(observer()).define(["md"], _28);
  main.variable(observer()).define(["md"], _29);
  main.variable(observer()).define(["md"], _30);
  main.variable(observer("formatPercentChange")).define("formatPercentChange", ["d3"], _formatPercentChange);
  main.variable(observer("formatRatio")).define("formatRatio", ["d3"], _formatRatio);
  main.variable(observer("color3a")).define("color3a", ["deaths","d3"], _color3a);
  main.variable(observer("color3b")).define("color3b", ["deaths","d3"], _color3b);
  main.variable(observer("color4")).define("color4", ["deaths","d3"], _color4);
  const child1 = runtime.module(define1);
  main.import("map", child1);
  main.import("legend", child1);
  main.import("names", child1);
  main.import("deaths", child1);
  main.import("format", child1);
  main.import("formatChange", child1);
  main.import("color", child1);
  main.import("color2", child1);
  main.import("d3", child1);
  return main;
}

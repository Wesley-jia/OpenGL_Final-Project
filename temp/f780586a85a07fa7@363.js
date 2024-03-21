// https://observablehq.com/@mbostock/mortality-due-to-alcohol-use-disorder@363
function _1(md){return(
md`# Deaths by Alcohol Use Disorder

Data: [Dwyer-Lindgren *et al.*](https://jamanetwork.com/journals/jama/fullarticle/2674665), [Institute for Health Metrics and Evaluation](https://vizhub.healthdata.org/subnational/usa)`
)}

function _2(md){return(
md`## 1980`
)}

function _3(map,color,deaths,names,format){return(
map(
  id => color(deaths.get(id)[0]),
  id => `${names.get(id)}
${format(deaths.get(id)[0])} deaths per 100,000 people`
)
)}

function _4(legend,color){return(
legend(color, "Deaths per 100,000 people")
)}

function _5(md){return(
md`## 2014`
)}

function _6(map,color,deaths,names,format){return(
map(
  id => color(deaths.get(id)[1]),
  id => `${names.get(id)}
${format(deaths.get(id)[1])} deaths per 100,000 people`
)
)}

function _7(legend,color){return(
legend(color, "Deaths per 100,000 people")
)}

function _8(md){return(
md`## 2014 – 1980`
)}

function _9(map,color2,deaths,names,formatChange,format){return(
map(
  id => color2(deaths.get(id)[1] - deaths.get(id)[0]),
  id => `${names.get(id)}
${formatChange(deaths.get(id)[1] - deaths.get(id)[0])} deaths per 100,000 people
${format(deaths.get(id)[0])} per 100,000 in 1980
${format(deaths.get(id)[1])} per 100,000 in 2014`
)
)}

function _10(legend,color2){return(
legend(color2, "Change in deaths per 100,000 people", "+d")
)}

function _11(md){return(
md`---

## Appendix`
)}

function _map(d3,DOM,counties,path,states){return(
function map(color, title, visibility = () => Promise.resolve()) {
  const svg = d3.select(DOM.svg(780, 500))
      .attr("viewBox", "90 6 780 500")
      .style("overflow", "visible")
      .style("width", "100%")
      .style("height", "auto")
      .style("transform", "translate3d(0, 0, 0)");

  visibility().then(() => {
    svg.selectAll("path")
      .data(counties)
      .enter().append("path")
        .attr("fill", d => color(d.properties.location_id))
        .attr("d", path)
        .on("mouseover", function() { d3.select(this).attr("stroke", "#000").raise(); })
        .on("mouseout", function() { d3.select(this).attr("stroke", null).lower(); })
      .append("title")
        .text(d => title(d.properties.location_id));

    svg.append("path")
        .attr("fill", "none")
        .attr("stroke", "#fff")
        .attr("d", path(states));
  });

  return svg.node();
}
)}

function _legend(d3,DOM,ramp){return(
function legend(color, title, format) {
  const width = 320;
  const height = 45;

  const svg = d3.select(DOM.svg(width, height))
      .style("overflow", "visible");

  const legend = svg.append("g")
      .attr("transform", `translate(0,20)`)
      .call(d3.axisBottom(color.copy().rangeRound(d3.quantize(d3.interpolate(0, width), color.domain().length))).tickSize(13).ticks(5, format))
      .call(g => g.selectAll(".tick line").attr("stroke", "#fff"))
      .call(g => g.select(".domain").remove());

  legend.insert("image", "*")
      .attr("width", width)
      .attr("height", 13)
      .attr("preserveAspectRatio", "none")
      .attr("xlink:href", ramp(color).toDataURL());

  legend.append("text")
      .attr("class", "caption")
      .attr("y", -6)
      .attr("fill", "#000")
      .attr("text-anchor", "start")
      .attr("font-weight", "bold")
      .text(title);

  return svg.node();
}
)}

function _formatChange(d3){return(
d3.format("+.1f")
)}

function _format(d3){return(
d3.format(".1f")
)}

function _color(d3,deaths){return(
d3.scaleSqrt()
    .interpolate(() => d3.interpolateBuPu)
    .domain([0, d3.max([...deaths.values()], d => Math.max(...d))])
)}

function _color2(deaths,d3)
{
  const values = [...deaths.values()];
  const max = Math.max(-d3.min(values, ([a, b]) => b - a), d3.max(values, ([a, b]) => b - a));
  return d3.scaleSqrt()
      .domain([-max, 0, max])
      .range([-1, 0, 1])
      .interpolate((a, b) => a < 0 
          ? t => d3.interpolateBlues(1 - t) 
          : t => d3.interpolateReds(t));
}


function _ramp(DOM,d3){return(
function ramp(color, n = 256) {
  const context = DOM.context2d(n, 1, 1);
  const x = d3.scaleLinear()
      .domain(d3.quantize(d3.interpolate(0, n - 1), color.domain().length))
      .range(color.domain());
  for (let i = 0; i < n; ++i) {
    context.fillStyle = color(x(i));
    context.fillRect(i, 0, 1, 1);
  }
  return context.canvas;
}
)}

function _path(d3){return(
d3.geoPath()
)}

async function _names(d3)
{
  const {locations: {flat}} = await d3.json("https://gist.githubusercontent.com/mbostock/df1b792d76fcb748056ff94b912e4bb8/raw/b1da4894cfb1e56a24129c27b39aa957d7f0c165/names.json");
  const map = new Map(Array.from(flat, ({location_id, name}) => [location_id, name]));
  for (const {location_id, parent_id, level, name} of flat) {
    if (level === 2) {
      map.set(location_id, `${name}, ${map.get(parent_id)}`);
    }
  }
  return map;
}


async function _deaths(d3)
{
  const {data} = await d3.json("https://gist.githubusercontent.com/mbostock/df1b792d76fcb748056ff94b912e4bb8/raw/b1da4894cfb1e56a24129c27b39aa957d7f0c165/deaths.json");
  return new Map(Array.from(data, ({location_id, value}) => [location_id, value]));
}


function _counties(topojson,topology){return(
topojson.feature(topology, topology.objects.counties).features
)}

function _states(topojson,topology){return(
topojson.mesh(topology, topology.objects.states, (a, b) => a !== b)
)}

async function _topology(d3){return(
(await d3.json("https://gist.githubusercontent.com/mbostock/df1b792d76fcb748056ff94b912e4bb8/raw/b1da4894cfb1e56a24129c27b39aa957d7f0c165/topology.json")).topology
)}

function _topojson(require){return(
require("topojson-client@3")
)}

function _d3(require){return(
require("d3@5")
)}

export default function define(runtime, observer) {
  const main = runtime.module();
  main.variable(observer()).define(["md"], _1);
  main.variable(observer()).define(["md"], _2);
  main.variable(observer()).define(["map","color","deaths","names","format"], _3);
  main.variable(observer()).define(["legend","color"], _4);
  main.variable(observer()).define(["md"], _5);
  main.variable(observer()).define(["map","color","deaths","names","format"], _6);
  main.variable(observer()).define(["legend","color"], _7);
  main.variable(observer()).define(["md"], _8);
  main.variable(observer()).define(["map","color2","deaths","names","formatChange","format"], _9);
  main.variable(observer()).define(["legend","color2"], _10);
  main.variable(observer()).define(["md"], _11);
  main.variable(observer("map")).define("map", ["d3","DOM","counties","path","states"], _map);
  main.variable(observer("legend")).define("legend", ["d3","DOM","ramp"], _legend);
  main.variable(observer("formatChange")).define("formatChange", ["d3"], _formatChange);
  main.variable(observer("format")).define("format", ["d3"], _format);
  main.variable(observer("color")).define("color", ["d3","deaths"], _color);
  main.variable(observer("color2")).define("color2", ["deaths","d3"], _color2);
  main.variable(observer("ramp")).define("ramp", ["DOM","d3"], _ramp);
  main.variable(observer("path")).define("path", ["d3"], _path);
  main.variable(observer("names")).define("names", ["d3"], _names);
  main.variable(observer("deaths")).define("deaths", ["d3"], _deaths);
  main.variable(observer("counties")).define("counties", ["topojson","topology"], _counties);
  main.variable(observer("states")).define("states", ["topojson","topology"], _states);
  main.variable(observer("topology")).define("topology", ["d3"], _topology);
  main.variable(observer("topojson")).define("topojson", ["require"], _topojson);
  main.variable(observer("d3")).define("d3", ["require"], _d3);
  return main;
}

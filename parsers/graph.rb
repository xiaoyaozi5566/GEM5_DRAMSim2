require 'rubyvis'
require_relative 'parsers'
include Parsers

module Parsers

def simple_bar data, o={}
  o = {
    x_labels: $specint,
    x_title:  "Percent Overhead",
    h: 200,
    w: 490
  }.merge(o)

  data_max = data.inject(data[0]) { |m, i| i > m ? i : m }

  h = o[:h]
  w = o[:w]
  x = pv.Scale.ordinal(pv.range(o[:x_labels].size)).
    split_banded(0, w, 4/5.0)
  y = pv.Scale.linear(0, data_max*1.1).range(0, h)

  vis = pv.Panel.new.
    width(w).
    height(h).
    bottom(20).
    left(50).
    right(5).
    top(10)
  
  # Bars
  bar = vis.add(pv.Bar).
    data(data).
    left(lambda { x.scale self.index } ).
    width(x.range_band).
    bottom(0).
    height(y)
  
  # Value lables on bars
  bar.anchor("top").add(pv.Label).
    text_style("white").
    text(lambda { |d| "%.2f" % d })
  
  # X-axis labels
  bar.anchor("bottom").add(pv.Label).
    text_align("center").
    text_baseline("top").
    text(lambda { o[:x_labels][self.index]})

  # Y-Axis Title
  vis.anchor("left").add(pv.Label).
    text("Percent Overhead").
    left(-30).
    text_align("center").
    font("12px sans-serif").
    text_angle(-Math::PI/2)

  # Y-Axis Ticks
  vis.add(pv.Rule).
    data(y.ticks(5)).
    bottom(y).
    stroke_style(lambda { |d| d==0 ? "#000" : "rgba(255,255,255,.3)" } ).
    add(pv.Rule).
    left(0).
    width(10).
    stroke_style("#000").
    anchor("left").add(pv.Label).
    text(y.tick_format)
  
  vis.render
  vis.to_svg
end

# Takes 2D array, outputs grouped bar graph
def grouped_bar data, o={}
  o = {
    x_labels: $specint,
    x_title:  "Percent Overhead",
    x_num_outer: data.size,
    x_num_inner: data[0].size,
    legend: [1,2,3,4,5,6],
    h: 200,
    w: 800 
  }.merge(o)

  data_max = data.inject(data[0][0]) do |max, i|
    rowmax = i.inject( i[0] ) { |m, j| j > m ? j : m }
    rowmax > max ? rowmax : max
  end

  h = o[:h]
  w = o[:w]
  x = pv.Scale.ordinal(pv.range(o[:x_num_outer])).
    split_banded(0, w-30, 7/8.0)
  y = pv.Scale.linear(0, data_max*1.1).range(0, h)

  vis = pv.Panel.new.
    width(w).
    height(h).
    bottom(20).
    left(50).
    right(5).
    top(10)
  
  # Bars
  colors=pv.Colors.category20()
  bar = vis.add(pv.Panel)
      .data(data)
      .left(lambda {x.scale(self.index)})
      .width(x.range_band)
    .add(pv.Bar)
      .data(lambda {|d| d})
      .left(lambda {self.index * x.range_band / o[:x_num_inner]})
      .width(x.range_band / o[:x_num_inner].to_f)
      .bottom(0)
      .height(y)
      .fillStyle(lambda { colors.scale(self.index)})

  # Value lables on bars
  # bar.anchor("top").add(pv.Label).
  #   text_style("white").
  #   text(lambda { |d| "%.2f" % d })
  
  # X-axis labels
  bar.parent.anchor("bottom").add(pv.Label).
    text_align("center").
    text_baseline("top").
    text(lambda { o[:x_labels][self.parent.index]})

  # Y-Axis Title
  vis.anchor("left").add(pv.Label).
    text("Percent Overhead").
    left(-40).
    text_align("center").
    font("12px sans-serif").
    text_angle(-Math::PI/2)

  # Y-Axis Ticks
  vis.add(pv.Rule).
    data(y.ticks(5)).
    bottom(y).
    stroke_style(lambda { |d| d==0 ? "#000" : "rgba(255,255,255,.3)" } ).
    add(pv.Rule).
    left(0).
    width(10).
    stroke_style("#000").
    anchor("left").add(pv.Label).
    text(y.tick_format)

  # Legend
  vis.add(pv.Dot).
    data(o[:legend]).
    bottom(lambda { |_| 15 + self.index * 10 }).
    shape_size(20).
    fillStyle(lambda { |_| colors.scale(self.index) }).
    left(w-28).
    anchor("left").
  add(pv.Label).
    text(lambda { |d| d.to_s }).
    text_align("right").
    left(w)
  
  vis.render
  vis.to_svg
end

end

if __FILE__ == $0
  data = pv.range(10).map { |d| rand*5 }
  data2 = pv.range(10).map do |i|
    pv.range(6).map { |j| rand*5 }
  end
  #open('foo.svg', 'w').write(simple_bar data)
  open('foo.svg', 'w').write(grouped_bar data2)
end

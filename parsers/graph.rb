require 'rubyvis'
require_relative 'parsers'
include Parsers

module Parsers

COLUMN_W = 288
COLUMN_H = 120

def simple_bar data, o={}
  o = {
    x_labels: $specint,
    x_title:  "Percent Overhead",
    # h: 200,
    # w: 490
    h: COLUMN_H,
    w: COLUMN_W
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
    text(o[:x_title]).
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
    # legend: [1,2,3,4,5,6],
    # h: 200,
    # w: 400 
    h: COLUMN_H,
    w: COLUMN_W
  }.merge(o)

  data_max = data.inject(data[0][0]) do |max, i|
    rowmax = i.inject( i[0] ) { |m, j| j > m ? j : m }
    rowmax > max ? rowmax : max
  end

  h = o[:h]
  w = o[:w]
  scale_w = o[:legend].nil? ? w : w-20
  x = pv.Scale.ordinal(pv.range(o[:x_num_outer])).
        split_banded(0, scale_w, 5/6.0)
  y = pv.Scale.linear(0, data_max*1.1).range(0, h-20)

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
      .left(lambda {self.index * x.range_band / o[:x_num_outer]})
      .width(x.range_band / o[:x_num_outer].to_f)
      .bottom(20)
      .height(y)
      .fillStyle(lambda { colors.scale(self.index)})

  # Value lables on bars
  # bar.anchor("top").add(pv.Label).
  #   text_style("white").
  #   text(lambda { |d| "%.2f" % d })
  
  # X-axis labels
  bar.parent.anchor("bottom").add(pv.Label).
    top(h-5).
    text_align("center").
    text_baseline("top").
    text_angle( Math::PI/6).
    font("9px sans-serif").
    text(lambda { o[:x_labels][self.parent.index]})

  # Y-Axis Title
  vis.anchor("left").add(pv.Label).
    text("Percent Overhead").
    left(-40).
    text_align("center").
    font("9px sans-serif").
    text_angle(-Math::PI/2)

  # Y-Axis Ticks
  vis.add(pv.Rule).
    data(y.ticks(5)).
    bottom(lambda { |d| y.scale(d) + 20 }).
    stroke_style(lambda { |d| d==0 ? "#000" : "rgba(255,255,255,.3)" } ).
    add(pv.Rule).
    left(0).
    width(10).
    stroke_style("#000").
    anchor("left").add(pv.Label).
    text(y.tick_format)

  # Legend
  unless o[:legend].nil?
    vis.add(pv.Dot).
      data(o[:legend]).
      bottom(lambda { |_| 35 + self.index * 10 }).
      shape_size(20).
      fillStyle(lambda { |_| colors.scale(self.index) }).
      left(w-18).
      anchor("left").
    add(pv.Label).
      text(lambda { |d| d.to_s }).
      text_align("right").
      left(w)
  end
  
  vis.render
  vis.to_svg
end

# Takes 2D array, outputs stacked bar graph
def stacked_bar data, o={}
  o = {
    x_labels: $specint,
    x_title:  "Percent Overhead",
    x_num_outer: data.size,
    x_num_inner: data[0].size,
    h: 200,
    w: 400 
  }.merge(o)

  data_max = data.inject(data[0][0]) do |max, i|
    rowmax = i.inject( i[0] ) { |m, j| j > m ? j : m }
    rowmax > max ? rowmax : max
  end

  h_max = data.transpose.inject(data[0].reduce(:+)) do |max, i|
    rowmax = i.reduce(:+)
    rowmax > max ? rowmax : max
  end

  h = o[:h]
  w = o[:w]
  x = pv.Scale.ordinal(pv.range(o[:x_num_inner])).
    split_banded(0, w-60, 4/5.0)
  y = pv.Scale.linear(0, h_max*1.1).range(0, h-20)

  vis = pv.Panel.new.
    width(w).
    height(h).
    bottom(20).
    left(50).
    right(5).
    top(10)

  puts data.to_s.blue
  puts data.transpose.to_s.blue
  puts data.transpose[0].to_s.blue

  #Stacks
  colors=pv.Colors.category20()
  bar = vis.add(pv.Layout.Stack).
    layers(data).
    x(lambda { x.scale(self.index) } ).
    y(lambda { |d| y.scale(d) } ).
    bottom(20).
    layer.add(pv.Bar).
    width(x.range_band)
  
  # X-axis labels
  bar.anchor("bottom").add(pv.Label).
    top(h-5).
    text_align("center").
    text_baseline("top").
    text_angle( Math::PI/6).
    font("9px sans-serif").
    text(lambda {
      self.parent.index == 0 ?
      o[:x_labels][self.index] : ""
    } )

  # Y-Axis Title
  vis.anchor("left").add(pv.Label).
    text("Percent Overhead").
    left(-40).
    text_align("center").
    font("9px sans-serif").
    text_angle(-Math::PI/2)

  # Y-Axis Ticks
  vis.add(pv.Rule).
    data(y.ticks(5)).
    bottom(lambda { |d| y.scale(d) + 20 }).
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
    bottom(lambda { |_| 35 + self.index * 10 }).
    shape_size(20).
    fillStyle(lambda { |_| colors.scale(self.index) }).
    left(w-58).
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

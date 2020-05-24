defmodule HammerTest do
  use ExUnit.Case
  doctest Hammer

  test "greets the world" do
    assert Hammer.hello() == :world
  end
end

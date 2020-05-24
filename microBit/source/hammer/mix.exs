defmodule Hammer.MixProject do
  use Mix.Project

  def project do
    [
      app: :hammer,
      version: "0.1.0",
      elixir: "~> 1.10",
      start_permanent: Mix.env() == :prod,
      deps: deps()
    ]
  end

  def application do
    [
      extra_applications: [:logger],
      mod: {Hammer.Application, []}
    ]
  end

  defp deps do
    [
      {:circuits_uart, "~> 1.4"},
      {:phoenix_gen_socket_client, "~> 2.1.1"},
      {:websocket_client, "~> 1.3"},
      {:poison, "~> 3.0"}
    ]
  end
end

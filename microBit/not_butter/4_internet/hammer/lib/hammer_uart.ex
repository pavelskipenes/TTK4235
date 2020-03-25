defmodule Hammer.UART do
  use GenServer

  alias Hammer.Message.SignOn
  alias Hammer.Message.State
  alias Hammer.Socket

  @token_stx 0x02
  @token_etx 0x03
  @type_sign_on 0x11
  @type_state 0x12
  @command_reset 0x21
  @command_turn_on 0x22
  @command_turn_off 0x23

  # Public API

  def start_link(_opts \\ []) do
    GenServer.start_link __MODULE__, :ok, [name: __MODULE__]
  end

  def reset do
    GenServer.call __MODULE__, :reset
  end

  def turn_on_matrix do
    GenServer.call __MODULE__, :turn_on
  end

  def turn_off_matrix do
    GenServer.call __MODULE__, :turn_off
  end

  # Callbacks

  def init(:ok) do
    {:ok, pid} = Circuits.UART.start_link()
    Circuits.UART.open(pid, "ttyACM0", speed: 9600, active: true)

    state = %{
      uart: pid,
      buffer: << >>
    }

    {:ok, state}
  end

  def handle_call(:reset, _from, state) do
    Circuits.UART.write state.uart, <<@command_reset>>
    {:reply, :ok, Map.put(state, :buffer, << >>)}
  end

  def handle_call(:turn_on, _from, state) do
    Circuits.UART.write state.uart, <<@command_turn_on>>
    {:reply, :ok, state}
  end

  def handle_call(:turn_off, _from, state) do
    Circuits.UART.write state.uart, <<@command_turn_off>>
    {:reply, :ok, state}
  end

  def handle_info({:circuits_uart, _device, byte}, state) do
    {buffer, messages} = parse(state.buffer <> byte)

    for m <- messages do
      case m do
        %State{} ->
          case Socket.send_state(m) do
            :reset ->
              Circuits.UART.write(state.uart, <<@command_reset>>)

            :ok ->
              :ok
          end

        %SignOn{} ->
          Socket.sign_on(m.id)
      end
    end

    new_state = Map.put state, :buffer, buffer
    {:noreply, new_state}
  end

  # Helpers

  defp parse(bytes) do
    parse(bytes, [])
  end

  defp parse(<<>>, messages) do
    {<<>>, messages}
  end

  defp parse(<<@token_stx, body :: binary>>, messages) do
    case body do
      <<>> ->
        {<<@token_stx>>, messages}

      <<@type_sign_on, tail :: binary>> ->
        parse_sign_on(tail, messages)

      <<@type_state, tail :: binary>> ->
        parse_state(tail, messages)

      <<_invalid_type, tail :: binary>> ->
        parse(tail, messages)
    end
  end

  defp parse(<<_not_stx, body :: binary>>, messages) do
    parse(body, messages)
  end

  defp parse_sign_on(
    <<
    id :: binary-size(6),
    @token_etx,
    tail :: binary
    >>,
    messages
  ) do
    parse(tail, messages ++ [%SignOn{id: id}])
  end

  defp parse_sign_on(
    <<
    _bad_byte,
    tail :: binary-size(6),
    rest :: binary
    >>,
    messages
  ) do
    parse(tail <> rest, messages)
  end

  defp parse_sign_on(incomplete_body, messages) do
    {<<@token_stx, @type_sign_on>> <> incomplete_body, messages}
  end

  defp parse_state(
    <<
    button_a,
    button_b,
    x_msb,
    x_lsb,
    y_msb,
    y_lsb,
    z_msb,
    z_lsb,
    @token_etx,
    tail :: binary
    >>,
    messages
  ) do
    x = signed_16(x_msb, x_lsb)
    y = signed_16(y_msb, y_lsb)
    z = signed_16(z_msb, z_lsb)

    state = %State{a: button_a, b: button_b, x: x, y: y, z: z}

    parse(tail, messages ++ [state])
  end

  defp parse_state(
    <<
    _bad_byte,
    tail :: binary-size(8),
    rest :: binary
    >>,
    messages
  ) do
    parse(tail <> rest, messages)
  end

  defp parse_state(incomplete_body, messages) do
    {<<@token_stx, @type_state>> <> incomplete_body, messages}
  end

  defp signed_16(msb, lsb) do
    case :erlang.band(msb, 0x80) do
      0x00 ->
        :binary.decode_unsigned(<<msb, lsb>>, :big)

      _signed ->
        bit_flip = <<:erlang.bnot(msb), :erlang.bnot(lsb)>>
        add_one = add_one_binary(bit_flip)
        -(:binary.decode_unsigned(add_one))
    end
  end

  defp add_one_binary(<<msb, lsb>>) do
    case lsb do
      0xff ->
        <<msb + 1, 0x00>>

      _ ->
        <<msb, lsb + 1>>
    end
  end

end

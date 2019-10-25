open Revery;
open Revery.UI;

type state = {
  ref: option(node),
  hasFocus: bool,
};

type action =
  | Focused(bool)
  | SetRef(node);

let reducer = (action, state) =>
  switch (action) {
  | Focused(v) => {...state, hasFocus: v}
  | SetRef(v) => {...state, ref: Some(v)}
  };
let component = React.component("KeyboardInput");

let createElement = (~children as _, ~quit, ~tapCallback, ()) =>
  component(hooks => {
    let (v, dispatch, hooks) =
      Hooks.reducer(
        ~initialState={ref: None, hasFocus: false},
        reducer,
        hooks,
      );

    let hooks =
      Hooks.effect(
        Always,
        () => {
          if (!v.hasFocus) {
            switch (v.ref) {
            | Some(v) => Focus.focus(v)
            | None => ()
            };
          };
          None;
        },
        hooks,
      );

    let onBlur = () => {
      dispatch(Focused(false));
    };

    let onFocus = () => {
      dispatch(Focused(true));
    };
    let respondToKeys = (e: NodeEvents.keyEventParams) =>
      Key.Keycode.(
        switch (e.keycode) {
        | v when v == escape || v == 113 => quit()
        | v when v == 32 => tapCallback()
        | _ => ()
        }
      );
    (
      hooks,
      <View
        ref={r => dispatch(SetRef(r))}
        onBlur
        onFocus
        style=Style.[position(`Absolute), width(1), height(1)]
        onKeyDown=respondToKeys
      />,
    );
  });

open Revery.UI;
open Revery.UI.Components;

module Styles = {
  let clickable =
    Style.[
      position(`Relative),
      backgroundColor(Theme.Color.grey2),
      justifyContent(`Center),
      alignItems(`Center),
      width(52),
      height(34),
      marginRight(4),
    ];

  let container =
    Style.[
      position(`Relative),
      justifyContent(`Center),
      alignItems(`Center),
    ];

  let text =
    Style.[
      color(Theme.Color.black),
      fontFamily(Theme.Typo.font),
      fontSize(Theme.Typo.size),
      lineHeight(Theme.Typo.lineHeight),
    ];
};

let component = React.component("TapButton");

let createElement = (~onClick, ~children as _, ()) => {
  let content =
    <Clickable style=Styles.clickable onClick>
      <View style=Styles.container>
        <Text style=Styles.text text="Tap" />
      </View>
    </Clickable>;

  component(hooks => (hooks, content));
};

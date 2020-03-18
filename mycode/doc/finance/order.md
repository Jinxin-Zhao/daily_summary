<!-- TOC -->

- [1. FOK & FAK](#1-fok-fak)

<!-- TOC -->

# 1. FOK & FAK
### definition
+ FOK(fill or kill) indicates that the pending orders are either to be traded or totally cancelled;
+ FAK(fill and kill) indicates that the pending orders are partially traded while the rest will be cancelled by trading system;
    you can set minimal trade volume selectively: 
    - if you set the minimal trade volume( minTradeV ) and the volumes of orders you send which can be traded >= minTradeV, then the rest orders will be cancelled. Otherwise, all the orders you send will be cancelled by trading system.
    - else the orders will be partially traded and be partially cancelled
### notes:
+ FOK & FAK instructions can't be used int the pre-market opening session.
+ FOK & FAK should coordinate the 'limit price instruction' offered by exchange.

### instance
+ fok:
![avatar](order_png/fok.png)
+ fak:
    - set minimal trade volume t0 30:
    ![avatar](order_png/fak_s.png)
    - minimal trade volume not set:
    ![avatar](order_png/fak.png)
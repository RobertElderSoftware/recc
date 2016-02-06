<?php
/*
    Copyright 2016 Robert Elder Software Inc.
    
    Licensed under the Apache License, Version 2.0 (the "License"); you may not 
    use this file except in compliance with the License.  You may obtain a copy 
    of the License at
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software 
    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the 
    License for the specific language governing permissions and limitations 
    under the License.
*/
?><html>
  <head>
    <script type="text/javascript" src="/emulators/javascript/op-cpu.js?cache_bust=<?php echo rand(); ?>"></script>
    <script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.4.4/jquery.min.js"></script>
    <script type="text/javascript" src="/emulators/javascript/testing.js?cache_bust=<?php echo rand(); ?>"></script>
    <style type="text/css">
      body{
        width: 1200px;
        font-family: "Courier New", Courier, monospace;
        font-size: 10px;
	color: #002b56;
	background-color: #e2f5fc;
      }
      .debug-explanation{
        color: #ee4136;
      }
      .next-instruction{
        font-size: 24px;
      }
      .op-name{
        color: #ee4136;
      }
      .pc-loc{
        font-weight: bold;
        color: #ee4136;
      }
      .register-container{
        width: 150px;
      }
      .location-container{
        width: 80px;
      }
      .instruction-container{
        width: 210px;
      }
      .binary-container{
        width: 410px;
      }
      .stack-value-container{
        width: 150px;
      }
      .stack-address-container{
        width: 150px;
      }
      .fr-bits td{
        padding: 5px;
        border: 1px solid #73c243;
        width: 100px;
        color: #f5b9ce;
        text-align: center;
      }
      .fr-bits td.active{
        background-color: #ee4136;
      }
      th{
        text-align: left;
      }
      .debug-view-area{
        display: none;
      }
    </style>
  </head>
  <body>
    <div>
      <button class="step">Step</button>
      <button class="resume">Resume</button>
      <button class="stop">Stop</button>
      <button style="display:none;" class="skip-to-watch-change">Skip to Watch Change</button>
      <input class="debug-mode" type="checkbox"><span class="debug-explanation">Full Debug View (10000x slower)</span>
    </div>
    <div>
      <button class="skip-to-breakpoint">Skip to Breakpoint</button>
      <input class="breakpointvalue" type="text" value="0x0">
      <select class="symbol-breakpoints">
        <option value="0x000006F4">0x000006F4 convertvalue0</option>
      </select>
    </div>
    <div>
      <span>Inspect Memory Location</span>
      <input class="inspectmemoryvalue" type="text" value="0x0">
    </div>
    <div class="debug-view-area">
      <table class="fr-bits">
        <tr>
          <td>
           DIV_ZERO 
          </td>
          <td>
            UART1_IN Ready
          </td>
          <td>
            UART1_OUT Ready
          </td>
          <td>
            UART1_IN Asserted
          </td>
          <td>
            UART1_IN Enabled
          </td>
          <td>
            UART1_OUT Asserted
          </td>
          <td>
            UART1_OUT Enabled
          </td>
          <td>
            TIMER1 Asserted
          </td>
          <td>
            TIMER1 Enabled
          </td>
          <td>
            RTE Bit
          </td>
          <td>
            Global Interrupt Enable
          </td>
          <td>
            Processor Halted
          </td>
        </tr>
      </table>
      <table>
        <tr>
          <th>
            Registers
          </th>
          <th>
            Location
          </th>
          <th>
            Instruction
          </th>
          <th>
            Binary Instruction Decomposition
          </th>
          <th>
            Value
          </th>
          <th>
            SP Address
          </th>
          <th>
            Value
          </th>
          <th>
            Memory Address
          </th>
          <th>
            Value
          </th>
        </tr>
        <tr>
          <td style="padding-right: 10px; border-right: 2px solid #000000;">
            <div class="register-container">
            </div>
          </td>
          <td style="padding-left: 10px;">
            <div class="location-container">
            </div>
          </td>
          <td>
            <div class="instruction-container">
            </div>
          </td>
          <td>
            <div class="binary-container">
            </div>
          </td>
          <td style="padding-right: 10px; border-right: 2px solid #000000;">
            <div class="value-container">
            </div>
          </td>
          <td style="padding-left: 10px;">
            <div class="stack-address-container">
            </div>
          </td>
          <td>
            <div class="stack-value-container">
            </div>
          </td>
          <td style="padding-left: 10px;">
            <div class="inspect-memory-address-container">
            </div>
          </td>
          <td>
            <div class="inspect-memory-value-container">
            </div>
          </td>
        </tr>
      </table>
    </div>
    <div class="test-status">
    </div>
    <div class="uart1-out">
    </div>
  </body>
</html>

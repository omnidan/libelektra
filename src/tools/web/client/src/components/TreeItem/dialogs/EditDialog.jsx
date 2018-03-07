/**
 * @file
 *
 * @brief dialog to modify the value of keys with sub-keys
 *
 * @copyright BSD License (see LICENSE.md or https://www.libelektra.org)
 */

import React, { Component } from 'react'

import Dialog from 'material-ui/Dialog'
import FlatButton from 'material-ui/FlatButton'
import TextField from 'material-ui/TextField'

import SavedIcon from '../SavedIcon.jsx'

import debounce from '../../debounce'

const DebouncedTextField = debounce(TextField)

export default class EditDialog extends Component {
  constructor (...args) {
    super(...args)
    this.state = { value: false, saved: false, timeout: false }
  }

  handleEdit = (value) => {
    const { onEdit } = this.props
    const { timeout } = this.state

    return onEdit(value)
      .then(() => {
        if (timeout) clearTimeout(timeout)
        this.setState({
          saved: true,
          timeout: setTimeout(() => this.setState({ saved: false }), 1500),
        })
      })
  }

  render () {
    const { item, value, open, onClose } = this.props
    const { path } = item

    const actions = [
      <FlatButton
        label="Done"
        primary={true}
        onTouchTap={onClose}
      />,
    ]

    return (
        <Dialog
          actions={actions}
          modal={false}
          open={open}
          onRequestClose={onClose}
        >
            <h1>Value of <b>{path}</b></h1>
            <div style={{ display: 'block' }}>
                <DebouncedTextField
                  floatingLabelText="value"
                  floatingLabelFixed={true}
                  onChange={value => this.setState({ value })}
                  onDebounced={this.handleEdit}
                  value={this.state.value === false ? value : this.state.value}
                />
                <SavedIcon saved={this.state.saved} />
            </div>
        </Dialog>
    )
  }
}
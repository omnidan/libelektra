/**
 * @file
 *
 * @brief sub-dialog to modify number metadata of keys
 *
 * @copyright BSD License (see LICENSE.md or https://www.libelektra.org)
 */

import React, { Component } from 'react'

import TextField from 'material-ui/TextField'
import FlatButton from 'material-ui/FlatButton'
import ContentAddIcon from 'material-ui/svg-icons/content/add'

import SavedIcon from '../SavedIcon.jsx'
import debounce from '../../debounce'

const DebouncedTextField = debounce(TextField)

export default class NumberSubDialog extends Component {
  constructor (...args) {
    super(...args)
    this.state = { ranges: [ ['', ''] ] }
  }

  addRange = () => {
    const { ranges } = this.state
    return [ ...ranges, ['', ''] ]
  }

  getMinMax = (index) => {
    const { ranges } = this.state
    const [ min, max ] = index >= ranges.length
      ? [ '', '' ]
      : ranges[index]
    return [ min, max ]
  }

  updateRangeMin = (index) => (e) => {
    const { value } = e && e.target
    const { ranges } = this.state
    const [ min, max ] = this.getMinMax(index)
    this.setState({ ranges: ranges.map((range, i) => {
      if (i === index) {
        return [ value, max ]
      }
      return range
    }) })
  }

  updateRangeMax = (index) => (e) => {
    const { value } = e && e.target
    const { ranges } = this.state
    const [ min, max ] = this.getMinMax(index)
    this.setState({ ranges: ranges.map((range, i) => {
      if (i === index) {
        return [ min, value ]
      }
      return range
    }) })
  }

  renderRangeField = (index) => {
    const [ min, max ] = this.getMinMax(index)
    return (
      <span key={index}>
        <TextField
          style={{ width: 32, marginRight: 8 }}
          floatingLabelText="min"
          value={min}
          onChange={this.updateRangeMin(index)}
        />
        {' - '}
        <TextField
          style={{ width: 32, marginLeft: 8 }}
          floatingLabelText="max"
          value={max}
          onChange={this.updateRangeMax(index)}
        />
      </span>
    )
  }

  render () {
    const { value, saved, onChange } = this.props
    const { ranges } = this.state

    return (
      <div style={{ display: 'block' }}>
        {ranges.map((_, i) => this.renderRangeField(i))}
        <FlatButton
          label="new range"
          icon={<ContentAddIcon />}
          primary
          style={{ marginLeft: 16 }}
          onClick={this.addRange}
        />
      </div>
    )
  }
}
